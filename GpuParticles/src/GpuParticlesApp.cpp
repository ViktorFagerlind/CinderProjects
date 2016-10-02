#include "cinder/app/AppNative.h"

#include "cinder/Rand.h"
#include "cinder/Surface.h"
#include "cinder/Text.h"
#include "cinder/Utilities.h"
#include "cinder/ImageIo.h"
#include "cinder/Perlin.h"
#include "cinder/CinderMath.h"

#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/Vbo.h"
#include "cinder/gl/GlslProg.h"

#include  "ShaderHelper.h"

using namespace ci;
using namespace ci::app;
using namespace std;

#define WIDTH     1024
#define HEIGHT    1024

#define PARTICLES 1024

class GpuParticles : public AppNative 
{
private:
  int           mPos;
  int           mBufferIn;
  int           mBufferOut;
  int           mFrameCounter;

  bool          mDrawTextures;
  bool          mIsFullScreen;
  bool          mCreateParticles;

  Perlin        mPerlin;

  vec3         mVertPos;

  gl::VboMesh   mVbo;
  gl::Fbo       mFbo[2];

  gl::GlslProg  mPosShader;
  gl::GlslProg  mVelShader;

  gl::Texture   mPosTex;
  gl::Texture   mVelTex;
  gl::Texture   mInfoTex;
  gl::Texture   mNoiseTex;

  gl::Texture   mSpriteTex;

  ivec2         m_mousePos;
  bool          m_mouseDown;

public:
  void setup ();
  void prepareSettings (Settings *settings);

  void mouseDown  (MouseEvent event);
  void mouseUp    (MouseEvent  event);

  void mouseDrag  (MouseEvent event);
  void mouseMove  (MouseEvent event);

  void keyDown    (KeyEvent event);
  void keyUp      (KeyEvent event);

  void update   ();
  void draw     ();
  void initFBO  ();
  void drawText ();
};

void GpuParticles::initFBO ()
{
  mPos = 0;
  mBufferIn = 0;
  mBufferOut = 1;

  mFbo[0].bindFramebuffer ();
  mFbo[1].bindFramebuffer ();

  //Positionen
  glDrawBuffer (GL_COLOR_ATTACHMENT0_EXT);

  gl::setMatricesWindow (mFbo[0].getSize (), false);
  gl::setViewport (mFbo[0].getBounds ());

  glClearColor (0.0f, 0.0f, 0.0f, 1.0f);
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  mPosTex.enableAndBind ();
  gl::draw (mPosTex, mFbo[0].getBounds ());
  mPosTex.unbind ();

  //velocity buffer
  glDrawBuffer (GL_COLOR_ATTACHMENT1_EXT);
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  mVelTex.enableAndBind ();
  gl::draw (mVelTex, mFbo[0].getBounds ());
  mVelTex.unbind ();

  //particle information buffer
  glDrawBuffer (GL_COLOR_ATTACHMENT2_EXT);
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  mInfoTex.enableAndBind ();
  gl::draw (mInfoTex, mFbo[0].getBounds ());
  mInfoTex.unbind ();

  mFbo[1].unbindFramebuffer ();
  mFbo[0].unbindFramebuffer ();

  mPosTex.disable ();
  mVelTex.disable ();
  mInfoTex.disable ();
}

void GpuParticles::prepareSettings (Settings *settings)
{
  settings->setWindowSize (WIDTH, HEIGHT);
  settings->setFrameRate (30.0f);
}

vec3 randPointFromSphere (float r)
{
  float U = Rand::randFloat (1.f);

  vec3  V = vec3 (Rand::randFloat (-1.f, 1.f),
                    Rand::randFloat (-1.f, 1.f),
                    Rand::randFloat (-1.f, 1.f));

  return r * cbrt (U) * V / V.length ();
}

void GpuParticles::setup ()
{
  gl::clear ();

  mPosShader = ShaderHelper::loadShader ("../Media/Shaders/GpuParticles/posVertShader.glsl",
                                         "../Media/Shaders/GpuParticles/posFragShader.glsl");

  mVelShader = ShaderHelper::loadShader ("../Media/Shaders/GpuParticles/velVertShader.glsl",
                                         "../Media/Shaders/GpuParticles/velFragShader.glsl");

  //controls
  mDrawTextures = false;
  mIsFullScreen = false;

  mFrameCounter = 0;

  mPerlin = Perlin (32, clock () * .1f);

  //initialize buffer
  Surface32f mPosSurface    = Surface32f (PARTICLES, PARTICLES, true);
  Surface32f mVelSurface    = Surface32f (PARTICLES, PARTICLES, true);
  Surface32f mInfoSurface   = Surface32f (PARTICLES, PARTICLES, true);
  Surface32f mNoiseSurface  = Surface32f (PARTICLES, PARTICLES, true);

  Surface32f::Iter iterator = mPosSurface.getIter ();


  while (iterator.line ())
  {
    while (iterator.pixel ())
    {

//      mVertPos = vec3 (Rand::randFloat (getWindowWidth ()) / (float)getWindowWidth (),
//        Rand::randFloat (getWindowHeight ()) / (float)getWindowHeight (), 0.0f);

      mVertPos = randPointFromSphere (0.5f) + vec3 (0.5f, 0.5f, 0.f);
      mVertPos.z = 0.f;

      //velocity
//      vec2 vel = vec2 (Rand::randFloat (-.005f, .005f), Rand::randFloat (-.005f, .005f));

      vec3 velTmp = randPointFromSphere (0.005f);
      vec2 vel = vec2 (velTmp.x, velTmp.y);

      float nX = iterator.x () * 0.005f;
      float nY = iterator.y () * 0.005f;
      float nZ = app::getElapsedSeconds () * 0.1f;
      vec3 v (nX, nY, nZ);
      float noise = mPerlin.fBm (v);

      float angle = noise * 15.0f;

      //vel = vec3 (cos (angle) * 6.28f, cos (angle) * 6.28f, 0.0f);

      //noise
      mNoiseSurface.setPixel (iterator.getPos (),
        Color (cos (angle) * Rand::randFloat (.00005f, .0002f), sin (angle) * Rand::randFloat (.00005f, .0002f), 0.0f));

      //position + mass
      mPosSurface.setPixel (iterator.getPos (),
        ColorA (mVertPos.x, mVertPos.y, mVertPos.z,
        Rand::randFloat (.00005f, .0002f)));
      //forces + decay
      mVelSurface.setPixel (iterator.getPos (), Color (vel.x, vel.y, Rand::randFloat (.01f, 1.00f)));

      //particle age
      mInfoSurface.setPixel (iterator.getPos (),
        ColorA (Rand::randFloat (.007f, 1.0f), 1.0f, 0.00f, 1.00f));

    }
  }

  //gl texture settings
  gl::Texture::Format tFormat;
  tFormat.setInternalFormat (GL_RGBA16F_ARB);

  gl::Texture::Format tFormatSmall;
  tFormat.setInternalFormat (GL_RGBA8);

  //mSpriteTex = gl::Texture (loadImage (loadFile ("../Media/Images/SoftBall.png")));
  mSpriteTex = gl::Texture (loadImage (loadFile ("../Media/Images/ring_flare.png")));

  mNoiseTex = gl::Texture (mNoiseSurface, tFormatSmall);
  mNoiseTex.setWrap (GL_REPEAT, GL_REPEAT);
  mNoiseTex.setMinFilter (GL_NEAREST);
  mNoiseTex.setMagFilter (GL_NEAREST);

  mPosTex = gl::Texture (mPosSurface, tFormat);
  mPosTex.setWrap (GL_REPEAT, GL_REPEAT);
  mPosTex.setMinFilter (GL_NEAREST);
  mPosTex.setMagFilter (GL_NEAREST);

  mVelTex = gl::Texture (mVelSurface, tFormat);
  mVelTex.setWrap (GL_REPEAT, GL_REPEAT);
  mVelTex.setMinFilter (GL_NEAREST);
  mVelTex.setMagFilter (GL_NEAREST);

  mInfoTex = gl::Texture (mInfoSurface, tFormatSmall);
  mInfoTex.setWrap (GL_REPEAT, GL_REPEAT);
  mInfoTex.setMinFilter (GL_NEAREST);
  mInfoTex.setMagFilter (GL_NEAREST);

  //initialize fbo
  gl::Fbo::Format format;
  format.enableDepthBuffer (false);
  format.enableColorBuffer (true, 3);
  format.setMinFilter (GL_NEAREST);
  format.setMagFilter (GL_NEAREST);
  format.setWrap (GL_CLAMP, GL_CLAMP);
  format.setColorInternalFormat (GL_RGBA16F_ARB);

  mFbo[0] = gl::Fbo (PARTICLES, PARTICLES, format);
  mFbo[1] = gl::Fbo (PARTICLES, PARTICLES, format);

  initFBO ();

  //fill dummy fbo
  vector<vec2> texCoords;
  vector<vec3> vertCoords, normCoords;
  vector<uint32_t> indices;

  gl::VboMesh::Layout layout;
  layout.setStaticIndices ();
  layout.setStaticPositions ();
  layout.setStaticTexCoords2d ();
  layout.setStaticNormals ();

  mVbo = gl::VboMesh (PARTICLES*PARTICLES, PARTICLES*PARTICLES, layout, GL_POINTS);

  for (int x = 0; x < PARTICLES; ++x) {
    for (int y = 0; y < PARTICLES; ++y) {
      indices.push_back (x * PARTICLES + y);
      texCoords.push_back (vec2 (x / (float)PARTICLES, y / (float)PARTICLES));
    }
  }

  mVbo.bufferIndices (indices);
  mVbo.bufferTexCoords2d (0, texCoords);

  setFrameRate (60.f);
  gl::disableVerticalSync ();

  m_mousePos = ivec2 (0, 0);
}

void GpuParticles::mouseDown (MouseEvent event)
{
  m_mousePos = event.getPos ();

  m_mouseDown = true;
}

void GpuParticles::mouseUp (MouseEvent  event)
{
  m_mouseDown = false;
}


void GpuParticles::mouseMove (MouseEvent event)
{
}

void GpuParticles::mouseDrag (MouseEvent event)
{
  m_mousePos = event.getPos ();
}

void GpuParticles::keyUp (KeyEvent event)
{

}

void GpuParticles::keyDown (KeyEvent event)
{

  if (event.getChar () == 't') {
    mDrawTextures = !mDrawTextures;
  }
  else if (event.getChar () == 'f') {
    setFullScreen (!isFullScreen ());
  }
  else if (event.getChar () == event.KEY_SPACE) {
    mCreateParticles = true;
  }
}
/**
here's where the magic happens
all calculations are done in update
**/
void GpuParticles::update ()
{

  mFbo[mBufferIn].bindFramebuffer ();

  //set viewport to fbo size
  gl::setMatricesWindow (mFbo[0].getSize (), false); // false to prevent vertical flipping
  gl::setViewport (mFbo[0].getBounds ());

  GLenum buffer[3] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_COLOR_ATTACHMENT2_EXT };
  glDrawBuffers (3, buffer);

  mFbo[mBufferOut].bindTexture (0, 0);
  mFbo[mBufferOut].bindTexture (1, 1);
  mFbo[mBufferOut].bindTexture (2, 2);

  mVelTex.bind (3);
  mPosTex.bind (4);
  mNoiseTex.bind (5);

  vec2 emissionPositions[2];
  emissionPositions[0] = vec2 ((float)m_mousePos.x / (float)getWindowWidth (),
                                (float)m_mousePos.y / (float)getWindowHeight ());
  emissionPositions[1] = vec2(1.f, 1.f) - emissionPositions[0];

  mVelShader.bind ();
  mVelShader.uniform ("positions",    0);
  mVelShader.uniform ("velocities",   1);
  mVelShader.uniform ("information",  2);
  mVelShader.uniform ("oVelocities",  3);
  mVelShader.uniform ("oPositions",   4);
  mVelShader.uniform ("noiseTex",     5);
  mVelShader.uniform ("emissionPositions", emissionPositions, 2);

  glBegin (GL_QUADS);
  glTexCoord2f (0.0f, 0.0f); glVertex2f (0.0f, 0.0f);
  glTexCoord2f (0.0f, 1.0f); glVertex2f (0.0f, PARTICLES);
  glTexCoord2f (1.0f, 1.0f); glVertex2f (PARTICLES, PARTICLES);
  glTexCoord2f (1.0f, 0.0f); glVertex2f (PARTICLES, 0.0f);
  glEnd ();

  mVelShader.unbind ();

  mFbo[mBufferOut].unbindTexture ();

  mVelTex.unbind ();
  mPosTex.unbind ();

  mFbo[mBufferIn].unbindFramebuffer ();

  mBufferIn = (mBufferIn + 1) % 2;
  mBufferOut = (mBufferIn + 1) % 2;

}

/**
the draw method displays the last filled buffer
**/
void GpuParticles::draw ()
{
  gl::setMatricesWindow (getWindowSize ());
  gl::setViewport (getWindowBounds ());

  gl::clear (ColorA (0.0f, 0.0f, 0.0f, 0.1f));

  gl::enableAlphaBlending ();
  gl::enableAdditiveBlending ();

  glDisable (GL_DEPTH_TEST);

  glEnable (GL_VERTEX_PROGRAM_POINT_SIZE);

  mFbo[mBufferIn].bindTexture (0, 0);
  mFbo[mBufferIn].bindTexture (1, 1);
  mFbo[mBufferIn].bindTexture (2, 2);

  mSpriteTex.bind (3);

  //Bewegungsshader
  mPosShader.bind ();
  mPosShader.uniform ("posTex", 0);
  mPosShader.uniform ("velTex", 1);
  mPosShader.uniform ("infTex", 2);
  mPosShader.uniform ("spriteTex", 3);

  mPosShader.uniform ("scale", (float)PARTICLES);

  gl::color (ColorA (1.0f, 1.0f, 1.0f, 1.0f));
  //glTranslatef(vec3(getWindowWidth() / 4  - PARTICLES,0.0f,0.0f));
  gl::pushMatrices ();

  glScalef (getWindowHeight () / (float)PARTICLES, getWindowHeight () / (float)PARTICLES, 1.0f);

  // draw particles
  gl::draw (mVbo);

  gl::popMatrices ();

  mPosShader.unbind ();

  mSpriteTex.unbind ();

  mFbo[mBufferIn].unbindTexture ();

  gl::disableAlphaBlending ();

  gl::color(Color(1,1,1));
  gl::setMatricesWindow (getWindowSize());
  drawText ();
}

void GpuParticles::drawText ()
{
  glClearColor (0.0f, 0.0f, 0.0f, 1.f);

  TextLayout layout;
  layout.setFont (Font ("Courier New", 14));
  layout.setColor (Color (1.0f, 1.0f, 1.0f));
  layout.addLine ("1 Million Particles");
  layout.addLine (" ");

  layout.setColor (Color (0.9f, 0.9f, 0.9f));

  layout.addLine ("F - switch to fullscreen");
  layout.addLine ("t - draw textures");

  char fps[50];
  sprintf (fps, "FPS: %.2f", getAverageFps ());

  char particleCount[50];
  sprintf (particleCount, "Particles: %d", PARTICLES*PARTICLES);

  layout.addLine (fps);
  layout.addLine (particleCount);

  glEnable (GL_TEXTURE_2D);
  gl::draw (layout.render (true), vec2 (getWindowWidth () - 150, 25));
  glDisable (GL_TEXTURE_2D);
}

CINDER_APP_BASIC (GpuParticles, RendererGl)