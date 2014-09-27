#include  "cinder/app/AppBasic.h"
#include  "cinder/Arcball.h"
#include  "cinder/Camera.h"
#include  "cinder/gl/GlslProg.h"
#include  "cinder/gl/Fbo.h"
#include  "cinder/gl/Texture.h"
#include  "cinder/gl/Vbo.h"
#include  "cinder/params/Params.h"
#include  "cinder/CinderMath.h"

#include  "MeshHelper.h"

#include  "ShaderHelper.h"

using  namespace  ci;

class  GpuParticlesApp : public  app::AppBasic
{
public:
  void						draw ();
  void						mouseDown (app::MouseEvent  event);
  void						mouseUp (app::MouseEvent  event);
  void						mouseDrag (app::MouseEvent  event);
  void						mouseWheel (app::MouseEvent  event);
  void						prepareSettings (app::AppBasic::Settings*  settings);
  void						resize ();
  void						setup ();
  void						update ();

private:
  Vec2i					  mSize;
  Vec2i					  mSizePrev;

  Arcball					mArcball;
  CameraPersp			mCamera;
  Vec3f					  mEyePoint;

  float						mLightAttenuationConstant;
  float						mLightAttenuationLinear;
  float						mLightAttenuationQuadratic;

  ColorAf					mLightAmbient;
  ColorAf					mLightDiffuse;
  Vec3f					  mLightPosition;
  ColorAf					mLightSpecular;
  float						mLightShine;

  float						mMaterialAmbient;
  float						mMaterialDiffuse;
  float						mMaterialEmissive;
  float						mMaterialSpecular;

  Vec2f					  mMouse;
  Vec2f					  mMouseVelocity;
  bool						mMouseDown;

  gl::Fbo					mFbo[2];
  size_t					mFboIndex;

  void						drawInstanced (const  gl::VboMeshRef&  vbo, size_t  count);
  gl::VboMeshRef	mMesh;

  gl::GlslProg		mGlslProgDraw;
  gl::GlslProg		mGlslProgGpuParticles0;
  gl::GlslProg		mGlslProgGpuParticles1;

  float						mBrushSize;
  gl::TextureRef	mTextureBrush;

  float						mFrameRate;
  bool						mFullScreen;
  bool						mFullScreenPrev;

  params::InterfaceGlRef	mParams;
};

/////////////////////////////

#include  "cinder/ImageIo.h"
#include  "Resources.h"

using  namespace  ci;
using  namespace  app;
using  namespace  std;

//  Color  attachments  to  hold  scene  data
static  const  GLenum  kColorAttachments[3] =
{
  GL_COLOR_ATTACHMENT0,	//  Position
  GL_COLOR_ATTACHMENT1,	//  Velocity
  GL_COLOR_ATTACHMENT2	//  Acceleration
};

void  GpuParticlesApp::draw ()
{
  //  We're  going  to  draw  new  data  onto  the  "ping"  FBO,
  //  using  the  "pong"  FBO's  textures  as  input
  size_t  pong = (mFboIndex + 1) % 2;

  //  Set  up  OpenGL  for  data
  gl::disableDepthRead ();
  gl::disableDepthWrite ();
  gl::setViewport (mFbo[mFboIndex].getBounds ());
  gl::color (ColorAf::white ());

  //  Draw  any  new  input  onto  the  acceleration  texture
  mFbo[pong].bindFramebuffer ();
  glDrawBuffer (GL_COLOR_ATTACHMENT2);
  if (mMouseDown)  
  {
    Vec2f  fboSize = Vec2f (mFbo[mFboIndex].getSize ());
    Vec2f  winSize = Vec2f (app::getWindowSize ());

    gl::setMatricesWindow (fboSize, true);

    Vec2f  brushSize = Vec2f::one ()  *  mBrushSize  *  fboSize;
    Vec2f  pos = (mMouse / winSize);
    pos.y = 1.0f - pos.y;
    pos *= fboSize;

    mGlslProgGpuParticles0.bind ();
    mGlslProgGpuParticles0.uniform ("color", ColorAf (mMouseVelocity.x, (mMouseVelocity.x + mMouseVelocity.y)/2, 1.0f - mMouseVelocity.y, 1.0f));
    mGlslProgGpuParticles0.uniform ("tex", 0);

    gl::enable (GL_TEXTURE_2D);
    mTextureBrush->bind ();
    gl::drawSolidRect (Rectf (pos - brushSize, pos + brushSize));
    mTextureBrush->unbind ();
    gl::disable (GL_TEXTURE_2D);

    mGlslProgGpuParticles0.unbind ();
  }
  mFbo[pong].unbindFramebuffer ();

  //  Now  let's  do  an  update  pass  in  GLSL
  mFbo[mFboIndex].bindFramebuffer ();
  gl::setMatricesWindow (mFbo[mFboIndex].getSize (), false);

  //  Bind  the  "pong"  textures  to  use  as  input  data
  for (int32_t i = 0; i < 3; ++i)  {
    mFbo[pong].bindTexture (i, i);
  }

  //  Set  up  shader  to  read  data  textures
  mGlslProgGpuParticles1.bind ();
  mGlslProgGpuParticles1.uniform ("offsets", 0);
  mGlslProgGpuParticles1.uniform ("velocities", 1);
  mGlslProgGpuParticles1.uniform ("acceleration", 2);

  //  Draw  a  rect  to  process  data
  glDrawBuffers (3, kColorAttachments);
  gl::drawSolidRect (mFbo[pong].getBounds ());

  //  Unbind  everything
  mFbo[pong].unbindTexture ();
  mGlslProgGpuParticles1.unbind ();
  mFbo[mFboIndex].unbindFramebuffer ();

  //  Swap  FBOs
  mFboIndex = pong;


  /////////////////////////////////

  //  Make  sure  we  have  data  to  work  with  before  we  draw  geometry
  if (mFbo[mFboIndex]                &&
      mFbo[mFboIndex].getTexture (0) &&
      mFbo[mFboIndex].getTexture (1))  
  {
    //  Set  up  window  for  3D  drawing
    gl::clear (Colorf (0.5f, 0.45f, 0.4f));
    gl::setViewport (getWindowBounds ());
    gl::setMatrices (mCamera);
    gl::enableDepthRead ();
    gl::enableDepthWrite ();
    gl::multModelView (mArcball.getQuat ());
    gl::color (ColorAf::black ());

    //  Set  up  shader  to  render  scene
    mGlslProgDraw.bind ();
    mGlslProgDraw.uniform ("Ax", mLightAmbient);
    mGlslProgDraw.uniform ("Ac", mLightAttenuationConstant);
    mGlslProgDraw.uniform ("Al", mLightAttenuationLinear);
    mGlslProgDraw.uniform ("Aq", mLightAttenuationQuadratic);
    mGlslProgDraw.uniform ("Dx", mLightDiffuse);
    mGlslProgDraw.uniform ("eyePoint", mEyePoint);
    mGlslProgDraw.uniform ("Ka", mMaterialAmbient);
    mGlslProgDraw.uniform ("Kd", mMaterialDiffuse);
    mGlslProgDraw.uniform ("Ke", mMaterialEmissive);
    mGlslProgDraw.uniform ("Ks", mMaterialSpecular);
    mGlslProgDraw.uniform ("lightPos", mLightPosition);
    mGlslProgDraw.uniform ("n", mLightShine);
    mGlslProgDraw.uniform ("offsets", 0);
    mGlslProgDraw.uniform ("projection", mCamera.getProjectionMatrix ());
    mGlslProgDraw.uniform ("size", Vec2f (mSize));
    mGlslProgDraw.uniform ("Sx", mLightSpecular);
    mGlslProgDraw.uniform ("t", (float)getElapsedSeconds ());

    //  Bind  textures  to  use  as  input  data
    for (int32_t i = 0; i <= 2; ++i)  {
      mFbo[mFboIndex].bindTexture (i, i);
    }

    //  Draw  instanced
    drawInstanced (mMesh, mSize.x  *  mSize.y);

    //  Finished  drawing
    mFbo[mFboIndex].unbindTexture ();
    mGlslProgDraw.unbind ();

    //  Draw  textures  so  we  can  see  what's  going  on  under  the  hood
    gl::setMatricesWindow (getWindowSize ());
    gl::disableDepthRead ();
    gl::disableDepthWrite ();
    gl::color (ColorAf::white ());
    gl::pushMatrices ();

    float  x = 20.0f;
    float  y = 440.0f;

    float  width = 64.0f;
    Area  srcArea (Vec2i::zero (), mSize);
    Rectf  destRect (x, y, x + width, y + width);

    gl::draw (mFbo[0].getTexture (0), srcArea, destRect);
    destRect.x1 += width;
    destRect.x2 += width;
    gl::draw (mFbo[1].getTexture (0), srcArea, destRect);

    destRect.y1 += width;
    destRect.y2 += width;
    destRect.x1 = x;
    destRect.x2 = x + width;
    gl::draw (mFbo[0].getTexture (1), srcArea, destRect);
    destRect.x1 += width;
    destRect.x2 += width;
    gl::draw (mFbo[1].getTexture (1), srcArea, destRect);

    destRect.y1 += width;
    destRect.y2 += width;
    destRect.x1 = x;
    destRect.x2 = x + width;
    gl::draw (mFbo[0].getTexture (2), srcArea, destRect);
    destRect.x1 += width;
    destRect.x2 += width;
    gl::draw (mFbo[1].getTexture (2), srcArea, destRect);
    gl::popMatrices ();
  }

  //  Draw  parameters
  if (getElapsedFrames () > 1) //  This  condition  prevents  a  memory  leak
    mParams->draw ();
}

void  GpuParticlesApp::drawInstanced (const  gl::VboMeshRef  &vbo, size_t  count)
{
  vbo->enableClientStates ();
  vbo->bindAllData ();
  glDrawElementsInstancedARB (vbo->getPrimitiveType (), vbo->getNumIndices (), GL_UNSIGNED_INT, (GLvoid*)(sizeof (uint32_t) * 0), count);
  gl::VboMesh::unbindBuffers ();
  vbo->disableClientStates ();
}

void  GpuParticlesApp::mouseDown (MouseEvent  event)
{
  if (event.isControlDown ())  
  {
    mArcball.mouseDown (event.getPos ());
  }
  else  
  {
    mMouseVelocity = Vec2f::zero ();
    mMouse = Vec2f (event.getPos ());
    mMouseDown = true;
  }
}

void  GpuParticlesApp::mouseDrag (MouseEvent  event)
{
  if (event.isControlDown ())  
  {
    mArcball.mouseDrag (event.getPos ());
  }
  else  
  {
    Vec2f  pos = Vec2f (event.getPos ());
    mMouseVelocity = pos - mMouse;
    mMouse = pos;
  }
}

void  GpuParticlesApp::mouseUp (MouseEvent  event)
{
  mMouseDown = false;
}

void  GpuParticlesApp::mouseWheel (MouseEvent  event)
{
  mEyePoint.z += -event.getWheelIncrement ()  *  5.0f;
}

void  GpuParticlesApp::prepareSettings (Settings*  settings)
{
  settings->setFrameRate (60.0f);
  settings->setWindowSize (1280, 720);
}

void  GpuParticlesApp::resize ()
{
  mCamera.setAspectRatio (getWindowAspectRatio ());
}


int64_t fact (int64_t x)
{
  int64_t r = 1LL;
  for (int64_t i = 1LL; i <= x; i++)
    r *= i;

  return r;
}

void  GpuParticlesApp::setup ()
{

  //  Load  shaders
  mGlslProgDraw = ShaderHelper::loadShader ("../Media/Shaders/GpuParticles/draw_vert.glsl",
                                            "../Media/Shaders/GpuParticles/draw_frag.glsl");

  mGlslProgGpuParticles0 = ShaderHelper::loadShader ("../Media/Shaders/GpuParticles/gpgpu_vert.glsl",
                                                     "../Media/Shaders/GpuParticles/gpgpu0_frag.glsl");

  mGlslProgGpuParticles1 = ShaderHelper::loadShader ("../Media/Shaders/GpuParticles/gpgpu_vert.glsl",
                                                     "../Media/Shaders/GpuParticles/gpgpu1_frag.glsl");

  //  Define  all  properties
  mArcball = Arcball (getWindowSize ());
  mBrushSize = 0.1f;
  mCamera = CameraPersp (getWindowWidth (), getWindowHeight (), 60.0f, 1.0f, 100000.0f);
  mEyePoint = Vec3f (0.0f, 20.0f, 256.0f);
  mFullScreen = isFullScreen ();
  mFullScreenPrev = mFullScreen;
  mLightAmbient = ColorAf::gray (0.1f);
  mLightAttenuationConstant = 0.1f;
  mLightAttenuationLinear = 0.01f;
  mLightAttenuationQuadratic = 0.001f;
  mLightDiffuse = ColorAf (0.9f, 0.3f, 0.667f);
  mLightPosition = Vec3f (11.38f, -1.39f, 59.74f);
  mLightSpecular = ColorAf::white ();
  mLightShine = 1.0f;
  mMaterialAmbient = 1.0f;
  mMaterialDiffuse = 1.0f;
  mMaterialEmissive = 0.0f;
  mMaterialSpecular = 1.0f;
  mMesh = gl::VboMesh::create (MeshHelper::createCube ());
  mMouseDown = false;
  mMouse = Vec2f::zero ();
  mMouseVelocity = Vec2f::zero ();
  mParams = params::InterfaceGl::create ("Params", Vec2i (250, 400));
  mSize = Vec2i (512, 512);
  mSizePrev = Vec2i::zero ();
  mTextureBrush = gl::Texture::create (loadImage (loadFile ("../Media/Images/SoftBall.png")));

  //  Set  up  arcball
  mArcball.setRadius ((float)getWindowHeight ()  *  0.5f);

  //  Set  up  parameters
  mParams->addParam ("Frame  rate", &mFrameRate, "", true);
  mParams->addParam ("Full  screen", &mFullScreen, "key=f");
  mParams->addButton ("Quit", bind (&GpuParticlesApp::quit, this), "key=q");

  mParams->addSeparator ("");
  mParams->addParam ("Brush  size", &mBrushSize, "min=0.0  max=1.0  step=0.001");
  mParams->addParam ("Size  X", &mSize.x, "min=1  max=1024  step=1");
  mParams->addParam ("Size  Y", &mSize.y, "min=1  max=1024  step=1");

  mParams->addSeparator ("");
  mParams->addParam ("Light  ambient", &mLightAmbient);
  mParams->addParam ("Light  att  const", &mLightAttenuationConstant, "min=0.0  max=1.0  step=0.001");
  mParams->addParam ("Light  att  line", &mLightAttenuationLinear, "min=0.0  max=1.0  step=0.0001");
  mParams->addParam ("Light  att  quad", &mLightAttenuationQuadratic, "min=0.0  max=1.0  step=0.00001");
  mParams->addParam ("Light  diffuse", &mLightDiffuse);
  mParams->addParam ("Light  position", &mLightPosition);
  mParams->addParam ("Light  specular", &mLightSpecular);
  mParams->addParam ("Light  shine", &mLightShine, "min=0.0  max=100000.0  step=1.0");

  mParams->addSeparator ("");
  mParams->addParam ("Material  ambient", &mMaterialAmbient, "min=0.0  max=1.0  step=0.001");
  mParams->addParam ("Material  diffuse", &mMaterialDiffuse, "min=0.0  max=1.0  step=0.001");
  mParams->addParam ("Material  emissive", &mMaterialEmissive, "min=0.0  max=1.0  step=0.001");
  mParams->addParam ("Material  specular", &mMaterialSpecular, "min=0.0  max=1.0  step=0.001");
}

void  GpuParticlesApp::update ()
{
  mFrameRate = getAverageFps ();

  //  Toggle  full  screen  mode
  if (mFullScreenPrev != mFullScreen)  {
    setFullScreen (mFullScreen);
    mFullScreenPrev = mFullScreen;
  }

  //  Reset  the  FBOs  if  our  grid  size  changes
  if (mSizePrev != mSize)  {

    //  Reset  FBO  index
    mFboIndex = 0;

    //  We're  going  to  create  two  FBOs,  each  with  three  color  attachments
    gl::Fbo::Format  format;
    format.enableColorBuffer (true, 3);
    format.setColorInternalFormat (GL_RGBA32F_ARB);
    for (size_t i = 0; i < 2; ++i)  {

      //  Create  the  FBO
      mFbo[i] = gl::Fbo (mSize.x, mSize.y, format);
      Area  bounds = mFbo[i].getBounds ();

      //  Bind  the  FBO  so  we  can  draw  on  it
      mFbo[i].bindFramebuffer ();

      //  Configure  the  view
      gl::setViewport (bounds);
      gl::setMatricesWindow (mSize);

      //  This  lets  us  target  the  three  color  attachments  together
      glDrawBuffers (3, kColorAttachments);

      //  Fill  in  all  attachments  with  grey
      gl::color (ColorAf::gray (0.5f));
      gl::drawSolidRect (bounds);

      //  Unbind  the  FBO  so  we  are  no  longer  drawing  to  it
      mFbo[i].unbindFramebuffer ();
    }

    mSizePrev = mSize;
  }

  //  Update  camera  position
  mCamera.lookAt (mEyePoint, Vec3f::zero ());

}

CINDER_APP_BASIC (GpuParticlesApp, RendererGl (RendererGl::AA_MSAA_32))
