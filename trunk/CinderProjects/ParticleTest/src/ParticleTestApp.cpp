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

#include "CommonModifier.h"
#include "FluidModifier.h"
#include "ColorModifier.h"
#include "GravityModifier.h"
#include "PointGravityModifier.h"
#include "VortexModifier.h"
#include "PerlinModifier.h"

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
  void prepareSettings( Settings *settings );
	void setup();
	void mouseDown( MouseEvent event );	
	void keyDown( KeyEvent event );

  static Vec2f mouseToWorld (const Vec2i& mousePosition, const int windowWidth, const int windowHeight);

	void updateCamera();

	void updateAttributes();

	void clearScreen();

	void update();
	void draw();

private:
	SystemAttributes createParticleSystem(size_t index);
  
private:
  Font mFont;

  CameraPersp mCam;

  ParticleSystemManager *mManager;

  SystemAttributes mSystems[MaxNofSystems_C];

  bool mSavingImages;

  gl::Fbo *mFrameBuffer;

  bool mSmoothFill;
};


void ParticleApp::prepareSettings (Settings *settings )
{
  settings->setWindowSize (1280, 768);

  mSavingImages = false;

  mSmoothFill = true;
}

void ParticleApp::updateCamera()
{
  Vec2i	mousePosition = getMousePos();

//  Vec3f cameraPosition = Vec3f ((float)mousePosition.x, (float)mousePosition.y, 1000.0f);
  Vec3f cameraPosition = Vec3f (0.0f, 0.0f, 1000.0f);

  mCam.lookAt(cameraPosition,  // camera
              Vec3f::zero(),   // center
              Vec3f::yAxis()); // up

  gl::setMatrices (mCam);

}

Vec2f ParticleApp::mouseToWorld (const Vec2i& mousePosition, const int windowWidth, const int windowHeight)
{
  return Vec2f ((float)mousePosition.x / (float)windowWidth  * 2000.0f - 1000.0f,
                (float)-mousePosition.y / (float)windowHeight * 1000.0f + 500.0f);
}

void ParticleApp::updateAttributes ()
{
  static int count = 0; 

  Vec2f mMousePosition             = mouseToWorld (getMousePos(), getWindowWidth(), getWindowHeight());
  static Vec2f mPrevMousePosition  = mMousePosition;

  Vec2f mouseSpeed   = mMousePosition - mPrevMousePosition;

  for (int index=0; index < MaxNofSystems_C; index++)
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
          r1 = Rand::randFloat(-.8f, .8f);
          r2 = Rand::randFloat(-.8f, .8f);
        }
        //sa.fm->applyMovement (Vec2f(0, -300), Vec2f(0, 10));

        sa.fm->applyMovement (Vec2f (170, -243.0f), Vec2f (r1, -1.0f));
        sa.fm->applyMovement (Vec2f (-170, -243.0f), Vec2f (r2, -1.0f));
      }
      break;

      case 2:
        sa.vm->setPosition (Vec3f (mMousePosition.x, mMousePosition.y, 0.0f));
      break;

      case 4:
        sa.fm->applyMovement (mMousePosition, mouseSpeed);
        sa.ae1->setPosition (Vec3f (mMousePosition.x, mMousePosition.y, 0.0f));
      break;

      case 5:
      {
        static float r1, r2, r3;
        if ((count % 100) == 0)
        {
          r1 = Rand::randFloat(-.8f, .8f);
          r2 = Rand::randFloat(-.8f, .8f);
          r3 = Rand::randFloat(-1.0f, 1.0f);
        }

        sa.fm->applyMovement (Vec2f (-800.0f, 200), Vec2f (2, r1));
        sa.fm->applyMovement (Vec2f (-800.0f, -200), Vec2f (2, r1));

        sa.fm->applyMovement (Vec2f::zero(), Vec2f (r3, r3));
      }
      break;
      case 6:
      {
        static float r1, r2, r3;
        if ((count % 100) == 0)
        {
          r1 = Rand::randFloat(-.8f, .8f);
          r2 = Rand::randFloat(-.8f, .8f);
          r3 = Rand::randFloat(-10.0f, 10.0f);
        }

        sa.fm->applyMovement (Vec2f (800.0f, 200), Vec2f (-2, r2));
        sa.fm->applyMovement (Vec2f (800.0f, -200), Vec2f (-2, r2));

        sa.fm->applyMovement (Vec2f::zero(), Vec2f (r3, r3));
      }
      break;
    }

  }


  mPrevMousePosition = mMousePosition;
  
  count++;
}

void ParticleApp::setup()
{
  mManager = new ParticleSystemManager();

  for (int i=0; i<MaxNofSystems_C; i++)
    mSystems[i].ps = NULL;

  mFont = Font( "Quicksand Book Regular", 12.0f );

  // Create frame buffer
  mFrameBuffer = new gl::Fbo (getWindowWidth(), getWindowHeight(), true, true, false);
/*
  gl::Fbo::Format format;
	format.setSamples( 4 ); // 4x antialiasing
  format.enableDepthBuffer (false);
	mFrameBuffer = new gl::Fbo (getWindowWidth(), getWindowHeight(), format);
*/

  mCam.setPerspective (60.0f, getWindowAspectRatio(), 100.0f, 5000.0f);

	glDepthMask (GL_FALSE);
	glDisable (GL_DEPTH_TEST);
	glEnable (GL_BLEND);

  setFrameRate (1000.0f);
}

void ParticleApp::update()
{
  updateAttributes();

  mManager->update();
}


void  ParticleApp::clearScreen()
{

  if (mSmoothFill)
  {
	  glBlendFunc (GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f (0, 0, 0, 0.1f);
    glBegin(GL_QUADS);
    glVertex3f (0.0f,             0.0f,               0.0f);
    glVertex3f (getWindowWidth(), 0.0f,               0.0f);
    glVertex3f (getWindowWidth(), getWindowHeight(),  0.0f);
    glVertex3f (0.0f,             getWindowHeight(),  0.0f);
    glEnd();
  }
  else
  {
    gl::clear (ColorAf(0, 0, 0, 0));
  }
}

void ParticleApp::draw()
{
  static size_t frameCount = 0;
  static size_t particleCount = 0;

  // Draw to frame buffer from now on
  mFrameBuffer->bindFramebuffer();

  gl::pushMatrices();

  clearScreen();

	glBlendFunc (GL_SRC_ALPHA, GL_ONE);

  updateCamera(); 

  mManager->draw();

  gl::popMatrices();

  frameCount++;
  if ((frameCount % 10) == 0)
  {
    particleCount = mManager->getCount();
//  gl::drawString( "FPS: " + toString((size_t)getAverageFps()) + "Count: " + toString(particleCount), Vec2f( 0.0f, 0.0f ), Color::white(), mFont );
    console() << "FPS: " << getAverageFps() << " Count: " << toString(particleCount) << std::endl;
  }

  mFrameBuffer->unbindFramebuffer();

  //gl::draw( myFbo->getTexture() );
  mFrameBuffer->blitToScreen (mFrameBuffer->getBounds(), getWindowBounds());

  if (mSavingImages)
  {
    static int currentImage = 0;
    if ((currentImage%2) == 0)
  	  writeImage (MyString::getFrameNumber ("saveImage_", currentImage/2) + ".png", mFrameBuffer->getTexture());	
	  currentImage++;
  }
}

void ParticleApp::mouseDown( MouseEvent event )
{
  size_t count = mManager->getCount();
}

void ParticleApp::keyDown( KeyEvent event )
{
  char c = event.getChar();

  if (c == 's')
  {
    mSavingImages = !mSavingImages;
    return;
  }

  if (c == 'f')
  {
    mSmoothFill = !mSmoothFill;
    return;
  }

  size_t index = (size_t) (c-'1');

  if (index > MaxNofSystems_C-1)
    return;

  if (mSystems[index].ps == NULL)
  {
    mSystems[index] = createParticleSystem(index);
    mManager->addParticleSystem (mSystems[index].ps);
  }
  else
  {
    mSystems[index].ps->kill();
    mSystems[index].ps = NULL;
  }
}

SystemAttributes ParticleApp::createParticleSystem(size_t index)
{
  SystemAttributes sa;

  switch(index)
  {
    case 0:
      sa.ps = new ParticleSystem("../Media/Images/smoke.png");

      sa.pe = new PointEmitter (10000,
                              Vec3f(-400.0f, 650, 0), //position
                              7.0f,  // particles per frame
                              30.0f,   // min size
                              30.0f,   // max size
                              Vec3f (0, 0, 0),     // baseVelocity
                              Vec3f (0.01f, 0.01f, 0));    // randVelocity
      sa.ps->addEmitter (sa.pe);

      sa.pe = new PointEmitter (10000,
                              Vec3f(0.0f, 650, 0), //position
                              7.0f,  // particles per frame
                              50.0f,   // min size
                              50.0f,   // max size
                              Vec3f (0, 0, 0),     // baseVelocity
                              Vec3f (0.01f, 0.01f, 0));    // randVelocity
      sa.ps->addEmitter (sa.pe);

      sa.pe = new PointEmitter (10000,
                              Vec3f(300.0f, 650, 0), //position
                              7.0f,  // particles per frame
                              40.0f,   // min size
                              40.0f,   // max size
                              Vec3f (0, 0, 0),     // baseVelocity
                              Vec3f (0.01f, 0.01f, 0));    // randVelocity
      sa.ps->addEmitter (sa.pe);

      sa.cm = new CommonModifier(0.2,    // lifeChange
                              1,    // relativeStartSize
                              1);  // relativeEndSize
      sa.ps->addModifier (sa.cm);

      sa.colorModifier = new ColorModifier (ColorAf(0.1, 0.3, 1.0, 1),   // startColor
                                         ColorAf(0.1, 0.3, 1.0, 1), // middleColor
                                         ColorAf(0.0, 0.0, 0.0, 1),   // endColor
                                         0.9f);                     // middleTime
      sa.ps->addModifier (sa.colorModifier);

      sa.gm = new GravityModifier(Vec3f(0,-0.04f,0));
      sa.ps->addModifier (sa.gm);

    	sa.pgm = new PointGravityModifier (Vec3f(-500, 300, 0.0f), // position
                                      250000.0f,             // strength
                                      1.5f,                  // max strength
                                      2000.0f);              // radius
      sa.ps->addModifier (sa.pgm);

    	sa.pgm = new PointGravityModifier (Vec3f(300, 140, 0.0f), // position
                                      350000.0f,             // strength
                                      1.5f,                  // max strength
                                      2000.0f);              // radius
      sa.ps->addModifier (sa.pgm);

      break;
    case 1:
      sa.ps = new ParticleSystem("../Media/Images/fire_alpha.png");

      sa.ie = new 	ImageEmitter(300000,
                              80.0f,
                              "../Media/Images/lines.png", 
                              Vec3f(0, -300, 0),
                              20.0f,   // min size
                              40.0f,   // max size
                              -2.0f,   // min vel
                              2.0f,    // max vel
							                1000,     // emitter width
							                500,     // emitter height
							                10);     // emitter depth
      sa.ps->addEmitter (sa.ie);
        
      sa.fm = new FluidModifier (100, Vec3f(0, 200, 0), 0.0001f, false, 2800, 1200);
      sa.ps->addModifier (sa.fm);

      sa.cm = new CommonModifier(1,    // lifeChange
                                 1,    // relativeStartSize
                                 0.2);  // relativeEndSize
      sa.ps->addModifier (sa.cm);

      sa.colorModifier = new ColorModifier (ColorAf(1, 1, 1, 0), // startColor
                                            ColorAf(1, 1, 1, 1), // middleColor
                                            ColorAf(1, 1, 1, 0), // endColor
                                            0.1f);            // middleTime
      sa.ps->addModifier (sa.colorModifier);

      sa.gm = new GravityModifier(Vec3f(0,0.1f,0));
      sa.ps->addModifier (sa.gm);

      break;
    case 2:
      sa.ps = new ParticleSystem("../Media/Images/smoke.png");

      sa.ae2 = new AreaEmitter (300000,
                                Vec3f(0, 0, 0),                //position
                                600, //particlesPerFrame,
  						                  1800, //width
  						                  1000, //height 
							                  2, // minParticleSize
							                  2, // maxParticleSize
							                  0.0f, // minParticleVelocity
							                  0.0f);  // maxParticleVelocity
      sa.ps->addEmitter (sa.ae2);

      sa.gm = new GravityModifier(Vec3f(0,-0.1f,0));
      sa.ps->addModifier (sa.gm);

      sa.cm = new CommonModifier (1,    // lifeChange
                                  1,    // relativeStartSize
                                  2);    // relativeEndSize
      sa.ps->addModifier (sa.cm);

    	sa.pgm = new PointGravityModifier (Vec3f(500, -200, 0), // position
                                      30000.0f,              // strength
                                      0.5f,                  // max strength
                                      3000.0f);              // radius
      sa.ps->addModifier (sa.pgm);

    	sa.pgm = new PointGravityModifier (Vec3f(-400, 300, 0), // position
                                      30000.0f,              // strength
                                      0.5f,                  // max strength
                                      3000.0f);              // radius
      sa.ps->addModifier (sa.pgm);

    	sa.pgm = new PointGravityModifier (Vec3f(0, 0, 0), // position
                                      10000.0f,              // strength
                                      0.5f,                  // max strength
                                      3000.0f);              // radius
      sa.ps->addModifier (sa.pgm);

      sa.colorModifier = new ColorModifier(ColorAf(1.0f, 0.1f, 0.1f, 0.0f), // startColor
                                           ColorAf(1.0f, 0.1f, 1.0f, 1.0f), // middleColor
                                           ColorAf(0.1f, 0.1f, 1.0f, 1.0f), // endColor
                                           0.5f);            // middleTime
      sa.ps->addModifier (sa.colorModifier);

//      pm = new PerlinModifier();
//      ps->addModifier (pm);

      sa.vm = new VortexModifier (Vec3f(-200.0f, -300.0f, 0),
                                  0.6f,   // strength
                                  0.01f,    // damping
                                  700,     // radius
                                  30 * 3.14f / 180.0f); // angle
      sa.ps->addModifier (sa.vm);

      sa.vm = new VortexModifier (Vec3f(300.0f, 100.0f, 0),
                                  1.5f,   // strength
                                  0.01f,    // damping
                                  400,     // radius
                                  30 * 3.14f / 180.0f); // angle
      sa.ps->addModifier (sa.vm);

      break;
    case 3:
      sa.ps = new ParticleSystem("../Media/Images/flare.png");

      sa.ae1 = new AreaEmitter (500000,
                                Vec3f(0, 0, 0),                //position
                                2000, //particlesPerFrame,
  						                  1800, //width
  						                  1000, //height 
							                  2, // minParticleSize
							                  2, // maxParticleSize
							                  0.0f, // minParticleVelocity
							                  0.0f);  // maxParticleVelocity
      sa.ps->addEmitter (sa.ae1);

      sa.cm = new CommonModifier(1,    // lifeChange
                              1,    // relativeStartSize
                              1);   // relativeEndSize
      sa.ps->addModifier (sa.cm);

      sa.vm = new VortexModifier (Vec3f(300.0f, 0.0f, 0),
                                  3.0f,   // strength
                                  0.01f,    // damping
                                  1000,     // radius
                                  -30 * 3.14f / 180.0f); // angle
      sa.ps->addModifier (sa.vm);

      sa.vm = new VortexModifier (Vec3f(-300.0f, -300.0f, 0),
                                  4.4f,   // strength
                                  0.01f,    // damping
                                  500,     // radius
                                  -30 * 3.14f / 180.0f); // angle
      sa.ps->addModifier (sa.vm);

      break;
    case 4:
      sa.ps = new ParticleSystem("../Media/Images/smoke.png");

      sa.ae1 = new AreaEmitter (100000,
                                Vec3f(0, 0, 0),                //position
                                20, //particlesPerFrame,
  						                  100, //width
  						                  100, //height 
							                  50, // minParticleSize
							                  50, // maxParticleSize
							                  0.1f, // minParticleVelocity
							                  0.1f);  // maxParticleVelocity
      sa.ps->addEmitter (sa.ae1);

      sa.fm = new FluidModifier (100, Vec3f::zero(), 0.001f, true, 1800, 1000);
      sa.ps->addModifier (sa.fm);

      sa.ae2 = new AreaEmitter (100000,
                                Vec3f(0, 0, 0),                //position
                                100, //particlesPerFrame,
  						                  1800, //width
  						                  1000, //height 
							                  3, // minParticleSize
							                  10, // maxParticleSize
							                  0.0f, // minParticleVelocity
							                  0.0f);  // maxParticleVelocity
      sa.ps->addEmitter (sa.ae2);

      sa.cm = new CommonModifier(0.3,    // lifeChange
                              1,    // relativeStartSize
                              1);   // relativeEndSize
      sa.ps->addModifier (sa.cm);

      sa.colorModifier = new ColorModifier(ColorAf(0.1, 0.3, 1.0, 1),   // startColor
                                           ColorAf(0.1, 0.3, 1.0, 1), // middleColor
                                           ColorAf(0.0, 0.0, 0.0, 1),   // endColor
                                           0.5f);                     // middleTime

      sa.ps->addModifier (sa.colorModifier);

      break;
    case 5:
      sa.ps = new ParticleSystem("../Media/Images/soapbubble_alpha.png");


      sa.ie = new 	ImageEmitter (300000,
                                  0.05f,
                                  "../Media/Images/lines.png", 
                                  Vec3f(0, 0, 0),
                                  100.0f,   // min size
                                  400.0f,   // max size
                                  0,   // min vel
                                  0,    // max vel
							                    1500,     // emitter width
							                    500,     // emitter height
							                    10);     // emitter depth
      sa.ps->addEmitter (sa.ie);
/*
      sa.ae1 = new AreaEmitter (100000,
                            Vec3f(-700, 0, 0),                //position
                            0.5f, //particlesPerFrame,
  						              300, //width
  						              1000, //height 
							              10, // minParticleSize
							              400, // maxParticleSize
							              0.1f, // minParticleVelocity
							              0.1f);  // maxParticleVelocity
      sa.ps->addEmitter (sa.ae1);*/

      sa.fm = new FluidModifier (100, Vec3f::zero(), 0.001f, false, 2000, 1000);
      sa.ps->addModifier (sa.fm);

      sa.cm = new CommonModifier (0.05,    // lifeChange
                                  1,    // relativeStartSize
                                  1);   // relativeEndSize
      sa.ps->addModifier (sa.cm);

      sa.colorModifier = new ColorModifier (ColorAf(1.0f, 0.01f, 0.01f, 0), // startColor
                                            ColorAf(1.0f, 0.01f, 0.01f, 0.4f), // middleColor
                                            ColorAf(1.0f, 0.01f, 0.01f, 0), // endColor
                                            0.5f);                     // middleTime

      sa.ps->addModifier (sa.colorModifier);

      break;
    case 6:
      sa.ps = new ParticleSystem("../Media/Images/smoke.png");

      sa.fm = new FluidModifier (100, Vec3f::zero(), 0.001f, false, 2000, 1000);
      sa.ps->addModifier (sa.fm);

      sa.ae2 = new AreaEmitter (100000,
                                Vec3f(700, 0, 0),                //position
                                2, //particlesPerFrame,
  						                  300, //width
  						                  1000, //height 
							                  100, // minParticleSize
							                  100, // maxParticleSize
							                  0.1f, // minParticleVelocity
							                  0.1f);  // maxParticleVelocity
      sa.ps->addEmitter (sa.ae2);

      sa.cm = new CommonModifier (0.4,    // lifeChange
                                  1,    // relativeStartSize
                                  1);   // relativeEndSize
      sa.ps->addModifier (sa.cm);

      sa.colorModifier = new ColorModifier (ColorAf(0.1f, 0.2f, 1, 0), // startColor
                                            ColorAf(0.1f, 0.2f, 1, 1), // middleColor
                                            ColorAf(0.1f, 0.2f, 1, 0), // endColor
                                            0.5f);                     // middleTime

      sa.ps->addModifier (sa.colorModifier);

      break;
    case 7:
      sa.ps = new ParticleSystem("../Media/Images/bacteria.png");

      sa.pe = new PointEmitter (100000,
                              Vec3f (0, -400, 0), //position
                              5.0f,  // particles per frame
                              6.0f,   // min size
                              6.0f,   // max size
                              Vec3f (3.5f, 2, 0),     // baseVelocity
                              Vec3f (1.0f, 1.0f, 0));    // randVelocity
      sa.ps->addEmitter (sa.pe);

      sa.cm = new CommonModifier (1.0f,    // lifeChange
                                  1,    // relativeStartSize
                                  1);  // relativeEndSize
      sa.ps->addModifier (sa.cm);

      sa.colorModifier = new ColorModifier (ColorAf(1.0f, 1.0f, 1.0f, 1.0f),   // startColor
                                            ColorAf(1.0f, 1.0f, 1.0f, 1.0f), // middleColor
                                            ColorAf(1.0f, 1.0f, 1.0f, 0.0f),   // endColor
                                            0.5f);                     // middleTime
      sa.ps->addModifier (sa.colorModifier);

      sa.gm = new GravityModifier(Vec3f(0, 0.08f,0));
      sa.ps->addModifier (sa.gm);

      sa.pm = new PerlinModifier (10.0f, 0.06, 0.001);
      sa.ps->addModifier (sa.pm);

      break;
    case 8:
      sa.ps = new ParticleSystem("../Media/Images/basic particle 1.png");

      sa.ie = new 	ImageEmitter(300000,
                              500.0f,
                              "../Media/Images/dark sun.png", 
                              Vec3f(0, 0, 0),
                              2.0f,   // min size
                              4.0f,   // max size
                              0.0f,   // min vel
                              0.0f,    // max vel
							                800,     // emitter width
							                800,     // emitter height
							                10);     // emitter depth
      sa.ps->addEmitter (sa.ie);

      sa.cm = new CommonModifier (3.0f,    // lifeChange
                                  1,    // relativeStartSize
                                  0.5);  // relativeEndSize
      sa.ps->addModifier (sa.cm);

      sa.colorModifier = new ColorModifier (ColorAf(1.0f, 0.0f, 1.0f, 0.0f),   // startColor
                                            ColorAf(0.0f, 1.0f, 0.0f, 1.0f), // middleColor
                                            ColorAf(1.0f, 0.0f, 1.0f, 0.0f),   // endColor
                                            0.5f);                     // middleTime
      sa.ps->addModifier (sa.colorModifier);

      sa.pm = new PerlinModifier (10.0f, 0.06, 0.001);
      sa.ps->addModifier (sa.pm);

      sa.vm = new VortexModifier (Vec3f::zero(),
                                  0.9f,   // strength
                                  0.01f,    // damping
                                  200,     // radius
                                  30 * 3.14f / 180.0f); // angle
      sa.ps->addModifier (sa.vm);

      break;
  }

  return sa;
}

CINDER_APP_BASIC (ParticleApp, RendererGl)
