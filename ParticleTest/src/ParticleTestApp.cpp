#include <iostream>
#include <sstream>

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/Utilities.h"
#include "cinder/gl/Fbo.h"

#include "cinder/Camera.h"

#include "MyStrings.h"
#include "ParticleSystemManager.h"
#include "ParticleSystem.h"

#include "AreaEmitter.h"
#include "PointEmitter.h"
#include "ImageEmitter.h"
#include "VideoEmitter.h"

#include "AnimParticleDrawer.h"
#include "TubeParticleDrawer.h"


#include "CommonModifier.h"
#include "FluidModifier.h"
#include "ColorModifier.h"
#include "GravityModifier.h"
#include "PointGravityModifier.h"
#include "VortexModifier.h"
#include "PerlinModifier.h"
#include "cinder/qtime/MovieWriter.h"
#include "cinder/ip/Flip.h"

using namespace ci;
using namespace ci::app;

static const size_t MaxNofSystems_C = 10;

class SystemAttributes
{
public:
  ParticleSystem        *ps;

  PointEmitter          *pe;
  AreaEmitter           *ae1;
  AreaEmitter           *ae2;
  ImageEmitter          *ie;
  VideoEmitter          *ve;

  CommonModifier        *cm;
  ColorModifier         *colorModifier;
  GravityModifier       *gm;
  VortexModifier        *vm;
  PerlinModifier        *pm;
  PointGravityModifier  *pgm;
  FluidModifier         *fm;
};

class ParticleApp : public AppBasic
{
public:
  void prepareSettings (Settings *settings);
  void setup ();
  void mouseDown (MouseEvent event);
  void keyDown (KeyEvent event);

  static vec2 mouseToWorld (const ivec2& mousePosition, const int windowWidth, const int windowHeight);

  void updateCamera ();

  void updateAttributes ();

  void clearScreen ();

  void update ();
  void draw ();

private:
  SystemAttributes createParticleSystem (size_t index);

private:
  Font mFont;

  CameraPersp mCam;

  SystemAttributes mSystems[MaxNofSystems_C];

  bool mSavingImages;

  gl::Fbo *mFrameBuffer;

  bool mSmoothFill;

  bool mSavingVideo;
  qtime::MovieWriter	mMovieWriter;

  bool mPaused;
};


void ParticleApp::prepareSettings (Settings *settings)
{
  settings->setWindowSize (1280, 768);

  mSavingImages = false;
  mSavingVideo = false;

  mSmoothFill = false;
}

void ParticleApp::updateCamera ()
{
  ivec2	mousePosition = getMousePos ();

  //  vec3 cameraPosition = vec3 ((float)mousePosition.x, (float)mousePosition.y, 1000.0f);
  vec3 cameraPosition = vec3 (0.0f, 0.0f, 1000.0f);

  mCam.lookAt (cameraPosition,  // camera
    vec3::zero (),   // center
    vec3::yAxis ()); // up

  gl::setMatrices (mCam);

}

vec2 ParticleApp::mouseToWorld (const ivec2& mousePosition, const int windowWidth, const int windowHeight)
{
  return vec2 ((float)mousePosition.x / (float)windowWidth  * 2000.0f - 1000.0f,
    (float)-mousePosition.y / (float)windowHeight * 1000.0f + 500.0f);
}

void ParticleApp::updateAttributes ()
{
  static int count = 0;

  vec2 mMousePosition = mouseToWorld (getMousePos (), getWindowWidth (), getWindowHeight ());
  static vec2 mPrevMousePosition = mMousePosition;

  vec2 mouseSpeed = mMousePosition - mPrevMousePosition;

  for (int index = 0; index < MaxNofSystems_C; index++)
  {
    SystemAttributes& sa = mSystems[index];

    if (sa.ps == NULL)
      continue;

    switch (index)
    {
    case 1:
    {
      static float r1, r2;
      if ((count % 100) == 0)
      {
        r1 = Rand::randFloat (-.8f, .8f);
        r2 = Rand::randFloat (-.8f, .8f);
      }
      //sa.fm->applyMovement (vec2(0, -300), vec2(0, 10));

      sa.fm->applyMovement (vec2 (170, -243.0f), vec2 (r1, -1.0f));
      sa.fm->applyMovement (vec2 (-170, -243.0f), vec2 (r2, -1.0f));
    }
      break;

    case 2:
      sa.vm->setPosition (vec3 (mMousePosition.x, mMousePosition.y, 0.0f));
      break;

    case 4:
      sa.fm->applyMovement (mMousePosition, mouseSpeed);
      sa.ae1->setPosition (vec3 (mMousePosition.x, mMousePosition.y, 0.0f));
      break;


    }

  }


  mPrevMousePosition = mMousePosition;

  count++;
}

void ParticleApp::setup ()
{
  mPaused = false;

  for (int i = 0; i < MaxNofSystems_C; i++)
    mSystems[i].ps = NULL;

  mFont = Font ("Quicksand Book Regular", 12.0f);

  // Create frame buffer
  //mFrameBuffer = new gl::Fbo (getWindowWidth(), getWindowHeight(), true, true, false);

  gl::Fbo::Format format;
  format.setSamples (4); // 4x antialiasing
  format.enableDepthBuffer (false);
  mFrameBuffer = new gl::Fbo (getWindowWidth (), getWindowHeight (), format);


  mCam.setPerspective (60.0f, getWindowAspectRatio (), 100.0f, 5000.0f);

  glDepthMask (GL_FALSE);
  glDisable (GL_DEPTH_TEST);
  glEnable (GL_BLEND);

  setFrameRate (1000.0f);
}

void ParticleApp::update ()
{
  updateAttributes ();

  if (mPaused)
    return;

  ParticleSystemManager::getSingleton ().update ();
}


void  ParticleApp::clearScreen ()
{

  if (mSmoothFill)
  {
    glBlendFunc (GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f (0, 0, 0, 0.1f);
    glBegin (GL_QUADS);
    glVertex3f (0.0f, 0.0f, 0.0f);
    glVertex3f (getWindowWidth (), 0.0f, 0.0f);
    glVertex3f (getWindowWidth (), getWindowHeight (), 0.0f);
    glVertex3f (0.0f, getWindowHeight (), 0.0f);
    glEnd ();
  }
  else
  {
    gl::clear (ColorAf (0, 0, 0, 0));
  }
}

void ParticleApp::draw ()
{
  static size_t frameCount = 0;
  static size_t particleCount = 0;

  // Draw to frame buffer from now on
  mFrameBuffer->bindFramebuffer ();

  gl::pushMatrices ();

  clearScreen ();

  glBlendFunc (GL_SRC_ALPHA, GL_ONE);

  updateCamera ();

  ParticleSystemManager::getSingleton ().draw ();

  gl::popMatrices ();

  frameCount++;
  if ((frameCount % 10) == 0)
  {
    particleCount = ParticleSystemManager::getSingleton ().getCount ();
    //  gl::drawString( "FPS: " + toString((size_t)getAverageFps()) + "Count: " + toString(particleCount), vec2( 0.0f, 0.0f ), Color::white(), mFont );
    console () << "FPS: " << getAverageFps () << " Count: " << toString (particleCount) << std::endl;
  }

  mFrameBuffer->unbindFramebuffer ();

  //gl::draw( mFrameBuffer->getTexture() );
  //  mFrameBuffer->blitToScreen (mFrameBuffer->getBounds(), getWindowBounds());
  gl::disableAlphaBlending ();
  gl::setViewport (getWindowBounds ());
  gl::setMatricesWindow (getWindowSize (), false);
  gl::draw (mFrameBuffer->getTexture (), mFrameBuffer->getTexture ().getBounds (), getWindowBounds ());
  gl::enableAlphaBlending ();

  // Save images or video and handle the flipping issue...
  if (mSavingImages || mSavingVideo)
  {
    static int currentImage = 0;
    if ((currentImage % 2) == 0)
    {
      Surface surface (mFrameBuffer->getTexture ());
      ip::flipVertical (&surface);

      if (mSavingImages)
        writeImage (MyString::getFrameNumber ("saveImage_", currentImage / 2) + ".png", surface);

      if (mSavingVideo)
        mMovieWriter.addFrame (surface);
    }
    currentImage++;
  }
}

void ParticleApp::mouseDown (MouseEvent event)
{
  size_t count = ParticleSystemManager::getSingleton ().getCount ();
}

void ParticleApp::keyDown (KeyEvent event)
{
  char c = event.getChar ();

  if (c == 'p')
  {
    mPaused = !mPaused;
  }

  if (c == 'a')
  {
    gl::enableVerticalSync (!gl::isVerticalSyncEnabled ());
    return;
  }

  if (c == 's')
  {
    mSavingImages = !mSavingImages;
    return;
  }


  if (c == 'v')
  {
    mSavingVideo = true;

    qtime::MovieWriter::Format format;

    std::string dir = "D:\\Libraries\\Documents\\Programming\\CppGraphics\\Cinder\\CinderProjects\\Release\\";

    if (qtime::MovieWriter::getUserCompressionSettings (&format, loadImage ("../Media/Images/flare.png"))) {
      mMovieWriter = qtime::MovieWriter (dir + "saveVideo.mov", 1280, 768, format);
    }
    return;
  }


  if (c == 'f')
  {
    mSmoothFill = !mSmoothFill;
    return;
  }

  size_t index = (size_t)(c - '1');

  if (index > MaxNofSystems_C - 1)
    return;

  if (mSystems[index].ps == NULL)
  {
    mSystems[index] = createParticleSystem (index);
    ParticleSystemManager::getSingleton ().addParticleSystem (mSystems[index].ps);
  }
  else
  {
    mSystems[index].ps->killSystemAndEmitters ();
    mSystems[index].ps = NULL;
  }
}

SystemAttributes ParticleApp::createParticleSystem (size_t index)
{
  SystemAttributes sa;

  switch (index)
  {
  case 0:
    sa.ps = new ParticleSystem ("../Media/Images/smoke.png");

    sa.pe = new PointEmitter (10000,
      vec3 (-400.0f, 650, 0), //position
      7.0f,  // particles per frame
      30.0f,   // min size
      30.0f,   // max size
      vec3 (0, 0, 0),     // baseVelocity
      0.f,            // minRandVelocity
      0.01f);    // maxRandVelocity
    sa.ps->addEmitter (sa.pe);

    sa.pe = new PointEmitter (10000,
      vec3 (0.0f, 650, 0), //position
      7.0f,  // particles per frame
      50.0f,   // min size
      50.0f,   // max size
      vec3 (0, 0, 0),     // baseVelocity
      0.f,            // minRandVelocity
      0.01f);    // maxRandVelocity
    sa.ps->addEmitter (sa.pe);

    sa.pe = new PointEmitter (10000,
      vec3 (300.0f, 650, 0), //position
      7.0f,  // particles per frame
      40.0f,   // min size
      40.0f,   // max size
      vec3 (0, 0, 0),     // baseVelocity
      0.f,            // minRandVelocity
      0.01f);    // maxRandVelocity
    sa.ps->addEmitter (sa.pe);

    sa.cm = new CommonModifier (0.2,    // lifeChange
      1,    // relativeStartSize
      1);  // relativeEndSize
    sa.ps->addModifier (sa.cm);

    sa.colorModifier = new ColorModifier (ColorAf (0.1, 0.3, 1.0, 1),   // startColor
      ColorAf (0.1, 0.3, 1.0, 1), // middleColor
      ColorAf (0.0, 0.0, 0.0, 1),   // endColor
      0.9f);                     // middleTime
    sa.ps->addModifier (sa.colorModifier);

    sa.gm = new GravityModifier (vec3 (0, -0.04f, 0));
    sa.ps->addModifier (sa.gm);

    sa.pgm = new PointGravityModifier (vec3 (-500, 300, 0.0f), // position
      250000.0f,             // strength
      1.5f,                  // max strength
      2000.0f);              // radius
    sa.ps->addModifier (sa.pgm);

    sa.pgm = new PointGravityModifier (vec3 (300, 140, 0.0f), // position
      350000.0f,             // strength
      1.5f,                  // max strength
      2000.0f);              // radius
    sa.ps->addModifier (sa.pgm);

    break;
  case 1:
    sa.ps = new ParticleSystem ("../Media/Images/fire_alpha.png");

    sa.ie = new 	ImageEmitter (300000,
      80.0f,
      "../Media/Images/lines.png",
      vec3 (0, -300, 0),
      20.0f,   // min size
      40.0f,   // max size
      -2.0f,   // min vel
      2.0f,    // max vel
      1000,     // emitter width
      500,     // emitter height
      10);     // emitter depth
    sa.ps->addEmitter (sa.ie);

    sa.fm = new FluidModifier (100, vec3 (0, 200, 0), 0.0001f, false, 2800, 1200);
    sa.ps->addModifier (sa.fm);

    sa.cm = new CommonModifier (1,    // lifeChange
      1,    // relativeStartSize
      0.2);  // relativeEndSize
    sa.ps->addModifier (sa.cm);

    sa.colorModifier = new ColorModifier (ColorAf (1, 1, 1, 0), // startColor
      ColorAf (1, 1, 1, 1), // middleColor
      ColorAf (1, 1, 1, 0), // endColor
      0.1f);            // middleTime
    sa.ps->addModifier (sa.colorModifier);

    sa.gm = new GravityModifier (vec3 (0, 0.1f, 0));
    sa.ps->addModifier (sa.gm);

    break;
  case 2:
    sa.ps = new ParticleSystem ("../Media/Images/smoke.png");

    sa.ae2 = new AreaEmitter (300000,
      vec3 (0, 0, 0),                //position
      600, //particlesPerFrame,
      1800, //width
      1000, //height 
      10,             // depth,
      2, // minParticleSize
      2, // maxParticleSize
      vec3 (0, 0, 0),     // baseVelocity
      0.0f);    // randVelocity


    sa.ps->addEmitter (sa.ae2);

    sa.gm = new GravityModifier (vec3 (0, -0.1f, 0));
    sa.ps->addModifier (sa.gm);

    sa.cm = new CommonModifier (1,    // lifeChange
      1,    // relativeStartSize
      2);    // relativeEndSize
    sa.ps->addModifier (sa.cm);

    sa.pgm = new PointGravityModifier (vec3 (500, -200, 0), // position
      30000.0f,              // strength
      0.5f,                  // max strength
      3000.0f);              // radius
    sa.ps->addModifier (sa.pgm);

    sa.pgm = new PointGravityModifier (vec3 (-400, 300, 0), // position
      30000.0f,              // strength
      0.5f,                  // max strength
      3000.0f);              // radius
    sa.ps->addModifier (sa.pgm);

    sa.pgm = new PointGravityModifier (vec3 (0, 0, 0), // position
      10000.0f,              // strength
      0.5f,                  // max strength
      3000.0f);              // radius
    sa.ps->addModifier (sa.pgm);

    sa.colorModifier = new ColorModifier (ColorAf (1.0f, 0.1f, 0.1f, 0.0f), // startColor
      ColorAf (1.0f, 0.1f, 1.0f, 1.0f), // middleColor
      ColorAf (0.1f, 0.1f, 1.0f, 1.0f), // endColor
      0.5f);            // middleTime
    sa.ps->addModifier (sa.colorModifier);

    //      pm = new PerlinModifier();
    //      ps->addModifier (pm);

    sa.vm = new VortexModifier (vec3 (-200.0f, -300.0f, 0),
      0.6f,   // strength
      0.01f,    // damping
      700,     // radius
      30 * 3.14f / 180.0f); // angle
    sa.ps->addModifier (sa.vm);

    sa.vm = new VortexModifier (vec3 (300.0f, 100.0f, 0),
      1.5f,   // strength
      0.01f,    // damping
      400,     // radius
      30 * 3.14f / 180.0f); // angle
    sa.ps->addModifier (sa.vm);

    break;
  case 3:
    sa.ps = new ParticleSystem ("../Media/Images/flare.png");

    sa.ae1 = new AreaEmitter (500000,
      vec3 (0, 0, 0),                //position
      2000, //particlesPerFrame,
      1800, //width
      1000, //height 
      10,             // depth,
      3, // minParticleSize
      3, // maxParticleSize
      vec3 (0, 0, 0),     // baseVelocity
      0.0f);    // randVelocity
    sa.ps->addEmitter (sa.ae1);

    sa.cm = new CommonModifier (1,    // lifeChange
      1,    // relativeStartSize
      1);   // relativeEndSize
    sa.ps->addModifier (sa.cm);

    sa.vm = new VortexModifier (vec3 (300.0f, 0.0f, 0),
      3.0f,   // strength
      0.01f,    // damping
      1000,     // radius
      -30 * 3.14f / 180.0f); // angle
    sa.ps->addModifier (sa.vm);

    sa.vm = new VortexModifier (vec3 (-300.0f, -300.0f, 0),
      4.4f,   // strength
      0.01f,    // damping
      500,     // radius
      -30 * 3.14f / 180.0f); // angle
    sa.ps->addModifier (sa.vm);

    break;
  case 4:
    sa.ps = new ParticleSystem ("../Media/Images/smoke.png");

    sa.ae1 = new AreaEmitter (100000,
      vec3 (0, 0, 0),                //position
      20, //particlesPerFrame,
      100, //width
      100, //height 
      10,             // depth,
      50, // minParticleSize
      50, // maxParticleSize
      vec3 (0, 0, 0),     // baseVelocity
      0.1f);    // randVelocity
    sa.ps->addEmitter (sa.ae1);

    sa.fm = new FluidModifier (100, vec3::zero (), 0.001f, true, 1800, 1000);
    sa.ps->addModifier (sa.fm);

    sa.ae2 = new AreaEmitter (100000,
      vec3 (0, 0, 0),                //position
      100, //particlesPerFrame,
      1800, //width
      1000, //height 
      10,             // depth,
      3, // minParticleSize
      10, // maxParticleSize
      vec3 (0, 0, 0),     // baseVelocity
      0.0f);    // randVelocity
    sa.ps->addEmitter (sa.ae2);

    sa.cm = new CommonModifier (0.3f,    // lifeChange
      1,    // relativeStartSize
      1);   // relativeEndSize
    sa.ps->addModifier (sa.cm);

    sa.colorModifier = new ColorModifier (ColorAf (0.1, 0.3, 1.0, 1),   // startColor
      ColorAf (0.1, 0.3, 1.0, 1), // middleColor
      ColorAf (0.0, 0.0, 0.0, 1),   // endColor
      0.5f);                     // middleTime

    sa.ps->addModifier (sa.colorModifier);

    break;
  case 5:
  {
    sa.ps = new ParticleSystem ("../Media/Movies/FireBall.png");

    shared_ptr<vector<SpriteData>> spriteDataRef (
      new vector<SpriteData> (SpriteDataParser::parseSpriteData (loadFile ("../Media/Movies/FireBall.xml"),
      SpriteSheet::FORMAT_TEXTUREPACKER_GENERIC_XML)));

    shared_ptr<AnimParticleDrawer> animDrawer = shared_ptr<AnimParticleDrawer> (new AnimParticleDrawer (spriteDataRef));

    sa.ae1 = new AreaEmitter (1000,
      vec3 (0, 0, 0),                //position
      1.2, // particlesPerFrame,
      50,      // width
      50,      // height 
      50,      // depth,
      120.0f,   // min size
      160.0f,   // max size
      vec3 (0, 0, 0),     // baseVelocity
      8.5f,    // randVelocity
      animDrawer);



    sa.ps->addEmitter (sa.ae1);

    sa.cm = new CommonModifier (0.9f,    // lifeChange
      1,    // relativeStartSize
      1);  // relativeEndSize
    sa.ps->addModifier (sa.cm);

    sa.colorModifier = new ColorModifier (ColorAf (1.0f, 1.0f, 1.0f, 0.1f),   // startColor
      ColorAf (1.0f, 1.0f, 1.0f, 0.6f), // middleColor
      ColorAf (1.0f, 1.0f, 1.0f, 0.1f),   // endColor
      0.5f);                     // middleTime
    sa.ps->addModifier (sa.colorModifier);

    sa.pgm = new PointGravityModifier (vec3 (0, 0, 0), 1000000.f, .06f, 1500.f);
    sa.ps->addModifier (sa.pgm);

    break;
  }
  case 6:
  {
    sa.ps = new ParticleSystem ("../Media/Images/Particle1.jpg");

    shared_ptr<TubeParticleDrawer> tubeDrawer = shared_ptr<TubeParticleDrawer> (new TubeParticleDrawer (10, 1));

    sa.pe = new PointEmitter (10000,
                              vec3 (0, 0, 0),        // position 
                              10.0f,                  // particles per frame
                              2.0f,                   // min size
                              4.0f,                   // max size
                              vec3 (0, 0, 0),        // baseVelocity
                              10.f,                    // minRandVelocity
                              15.f,                   // maxRandVelocity
                              tubeDrawer);
    sa.ps->addEmitter (sa.pe);

    sa.cm = new CommonModifier (1.0f,     // lifeChange
                                0.2f,     // relativeStartSize
                                1.0);     // relativeEndSize
    sa.ps->addModifier (sa.cm);

    sa.colorModifier = new ColorModifier (ColorAf (.6f, .5f, .9f, 1.f),   // startColor
                                          ColorAf (.5f, .6f, .9f, 1.f),   // middleColor
                                          ColorAf (.2f, .8f, .9f, 0.f),   // endColor
                                          0.7f);                          // middleTime
    sa.ps->addModifier (sa.colorModifier);

    sa.pm = new PerlinModifier (10.0f, 0.1, 0.003);
    sa.ps->addModifier (sa.pm);

    break;
  }
  case 7:
  {
    sa.ps = new ParticleSystem ("../Media/Movies/InkParticle.png");

    shared_ptr<vector<SpriteData>> spriteDataRef (
      new vector<SpriteData> (SpriteDataParser::parseSpriteData (loadFile ("../Media/Movies/InkParticle.xml"),
      SpriteSheet::FORMAT_TEXTUREPACKER_GENERIC_XML)));

    shared_ptr<AnimParticleDrawer> animDrawer = shared_ptr<AnimParticleDrawer> (new AnimParticleDrawer (spriteDataRef));

    sa.ae1 = new AreaEmitter (1000,
      vec3 (0, -200, 0),                //position
      1.2, // particlesPerFrame,
      260,      // width
      160,      // height 
      160,      // depth,
      100.0f,   // min size
      150.0f,   // max size
      vec3 (0, 10, 0),     // baseVelocity
      2.0f,    // randVelocity
      animDrawer);


    sa.ps->addEmitter (sa.ae1);

    sa.cm = new CommonModifier (0.5f,    // lifeChange
      1,    // relativeStartSize
      1);  // relativeEndSize
    sa.ps->addModifier (sa.cm);

    sa.colorModifier = new ColorModifier (ColorAf (1.0f, 1.0f, 1.0f, 0.2f),   // startColor
      ColorAf (1.0f, 1.0f, 1.0f, 0.2f), // middleColor
      ColorAf (1.0f, 1.0f, 1.0f, 0.0f),   // endColor
      0.5f);                     // middleTime
    sa.ps->addModifier (sa.colorModifier);

    sa.gm = new GravityModifier (vec3 (0, -0.1f, 0));
    sa.ps->addModifier (sa.gm);

    break;
  }
  case 8:
    sa.ps = new ParticleSystem ("../Media/Images/basic particle 1.png");

    sa.ve = new 	VideoEmitter (500000,
      3600.0f,
      vec3 (0, 0, 0),
      2.0f,   // min size
      2.0f,   // max size
      -0.1f,   // min vel
      0.1f,    // max vel
      1600,     // emitter width
      1000,     // emitter height
      10);     // emitter depth
    sa.ps->addEmitter (sa.ve);

    sa.cm = new CommonModifier (2.0f,    // lifeChange
      1.0f,    // relativeStartSize
      1.0f);  // relativeEndSize
    sa.ps->addModifier (sa.cm);

    sa.ps->addModifier (sa.ve);

    //      sa.pm = new PerlinModifier (10.0f, 0.01, 0.001);
    //      sa.ps->addModifier (sa.pm);

    /*
          sa.colorModifier = new ColorModifier (ColorAf(1, 1, 1, 0), // startColor
          ColorAf(1, 1, 1, 1), // middleColor
          ColorAf(1, 1, 1, 0), // endColor
          0.1f);            // middleTime
          sa.ps->addModifier (sa.colorModifier);
          */

    break;
  }

  return sa;
}

CINDER_APP_BASIC (ParticleApp, RendererGl)
