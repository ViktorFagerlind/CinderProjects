#include "cinder/app/AppBasic.h"
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

class ParticleApp : public AppBasic 
{
public:
  void prepareSettings( Settings *settings );
	void setup();
	void mouseDown( MouseEvent event );	
	void keyDown( KeyEvent event );

	void updateCamera();

	void update();
	void draw();

private:
	ParticleSystem* createParticleSystem(size_t index);
  
private:
  Font mFont;

  CameraPersp mCam;

  ParticleSystemManager *mManager;

  ParticleSystem *mSystems[MaxNofSystems_C];
};


void ParticleApp::prepareSettings( Settings *settings )
{
  settings->setWindowSize (1280, 768);
}

void ParticleApp::setup()
{
  mManager = new ParticleSystemManager();

  for (int i=0; i<MaxNofSystems_C; i++)
    mSystems[i] = NULL;

  mFont = Font( "Quicksand Book Regular", 12.0f );

  mCam.setPerspective (60.0f, getWindowAspectRatio(), 100.0f, 5000.0f);
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

void ParticleApp::update()
{
  mManager->update();
  Sleep(0);
}

void ParticleApp::draw()
{
  static size_t frameCount = 0;
  static size_t particleCount = 0;

  gl::pushMatrices();

  updateCamera(); 

	// clear out the window with black
	glClearColor (0, 0, 0, 0);

	glClear (GL_COLOR_BUFFER_BIT);

	glDepthMask (GL_FALSE);
	glDisable (GL_DEPTH_TEST);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE);

  mManager->draw();

  gl::popMatrices();

  if ((frameCount % 10) == 0)
    particleCount = mManager->getCount();
  frameCount++;
  gl::drawString( "FPS: " + MyString::toString((size_t)getAverageFps()) + "Count: " + MyString::toString(particleCount), Vec2f( 0.0f, 0.0f ), Color::white(), mFont );

}

void ParticleApp::mouseDown( MouseEvent event )
{
  size_t count = mManager->getCount();
}

void ParticleApp::keyDown( KeyEvent event )
{
  char c = event.getChar();
  size_t index = (size_t) (c-'1');

  if (index > MaxNofSystems_C-1)
    return;

  if (mSystems[index] == NULL)
  {
    mSystems[index] = createParticleSystem(index);
    mManager->addParticleSystem (mSystems[index]);
  }
  else
  {
    mSystems[index]->kill();
    mSystems[index] = NULL;
  }
}

ParticleSystem* ParticleApp::createParticleSystem(size_t index)
{
  static PointEmitter *pe = NULL;
  static AreaEmitter  *ae = NULL;
  static AreaEmitter  *te = NULL;
  static ImageEmitter *ie = NULL;

  CommonModifier        *cm;
  ColorModifier         *colorModifier;
  GravityModifier       *gm;
  VortexModifier        *vm;
  PerlinModifier        *pm;
  ParticleSystem        *ps;
  PointGravityModifier  *pgm;
  FluidModifier         *fm;

  switch(index)
  {
    case 0:
      ps = new ParticleSystem("../Media/Images/smoke.png");

      pe = new PointEmitter (10000,
                              Vec3f(-400.0f, 650, 0), //position
                              7.0f,  // particles per frame
                              30.0f,   // min size
                              30.0f,   // max size
                              -0.1f,     // min vel
                              0.01f);    // max vel
      ps->addEmitter (pe);

      pe = new PointEmitter (10000,
                              Vec3f(0.0f, 650, 0), //position
                              7.0f,  // particles per frame
                              50.0f,   // min size
                              50.0f,   // max size
                              -0.1f,     // min vel
                              0.01f);    // max vel
      ps->addEmitter (pe);

      pe = new PointEmitter (10000,
                              Vec3f(300.0f, 650, 0), //position
                              7.0f,  // particles per frame
                              40.0f,   // min size
                              40.0f,   // max size
                              -0.01f,     // min vel
                              0.01f);    // max vel
      ps->addEmitter (pe);

      cm = new CommonModifier(0.2,    // lifeChange
                              1,    // relativeStartSize
                              1);  // relativeEndSize
      ps->addModifier (cm);

      colorModifier = new ColorModifier (ColorAf(0.1, 0.3, 1.0, 1),   // startColor
                                         ColorAf(0.1, 0.3, 1.0, 1), // middleColor
                                         ColorAf(0.0, 0.0, 0.0, 1),   // endColor
                                         0.9f);                     // middleTime
      ps->addModifier (colorModifier);

      gm = new GravityModifier(Vec3f(0,-0.04f,0));
      ps->addModifier (gm);

    	pgm = new PointGravityModifier (Vec3f(-500, 300, 0.0f), // position
                                      250000.0f,             // strength
                                      1.5f,                  // max strength
                                      2000.0f);              // radius
      ps->addModifier (pgm);

    	pgm = new PointGravityModifier (Vec3f(300, 140, 0.0f), // position
                                      350000.0f,             // strength
                                      1.5f,                  // max strength
                                      2000.0f);              // radius
      ps->addModifier (pgm);

      return ps;
    case 1:
      ps = new ParticleSystem("../Media/Images/fire.png");

      ie = new 	ImageEmitter(300000,
                              100.0f,
                              "../Media/Images/lines.png", 
                              Vec3f::zero(),
                              20.0f,   // min size
                              40.0f,   // max size
                              -2.0f,   // min vel
                              2.0f,    // max vel
							                1000,     // emitter width
							                500,     // emitter height
							                10);     // emitter depth
      ps->addEmitter (ie);
        
      fm = new FluidModifier(this, ie, 100, Vec3f::zero(), 1800, 1000);
      ps->addModifier (fm);

      cm = new CommonModifier(1,    // lifeChange
                              1,    // relativeStartSize
                              0);  // relativeEndSize
      ps->addModifier (cm);

      gm = new GravityModifier(Vec3f(0,0.1f,0));
      ps->addModifier (gm);

      return ps;
    case 2:
      ps = new ParticleSystem("../Media/Images/smoke.png");

      ae = new AreaEmitter(300000,
                            Vec3f(0, 0, 0),                //position
                            800, //particlesPerFrame,
  						              1800, //width
  						              1000, //height 
							              4, // minParticleSize
							              4, // maxParticleSize
							              0.0f, // minParticleVelocity
							              0.0f);  // maxParticleVelocity
      ps->addEmitter (ae);

      gm = new GravityModifier(Vec3f(0,-0.1f,0));
      ps->addModifier (gm);

      cm = new CommonModifier(1,    // lifeChange
                              1,    // relativeStartSize
                              2);    // relativeEndSize
      ps->addModifier (cm);

      colorModifier = new ColorModifier (ColorAf(0.9, 0.1, 0.9, 0), // startColor
                                         ColorAf(0.5, 0.5, 0.1, 1), // middleColor
                                         ColorAf(0.1, 0.9, 0.9, 0), // endColor
                                         0.5f);            // middleTime
      ps->addModifier (colorModifier);

//      pm = new PerlinModifier();
//      ps->addModifier (pm);

      vm = new VortexModifier (Vec3f(300.0f, 100.0f, 0),
                                1.2f,   // strength
                                0.01f,    // damping
                                700,     // radius
                                -30 * 3.14f / 180.0f); // angle
      ps->addModifier (vm);

      vm = new VortexModifier (Vec3f(-200.0f, -300.0f, 0),
                                0.6f,   // strength
                                0.01f,    // damping
                                700,     // radius
                                -30 * 3.14f / 180.0f); // angle
      ps->addModifier (vm);

      return ps;
    case 3:
      ps = new ParticleSystem("../Media/Images/flare.png");

      te = new AreaEmitter(500000,
                            Vec3f(0, 0, 0),                //position
                            5000, //particlesPerFrame,
  						              1800, //width
  						              1000, //height 
							              1, // minParticleSize
							              1, // maxParticleSize
							              0.0f, // minParticleVelocity
							              0.0f);  // maxParticleVelocity
      ps->addEmitter (te);

      cm = new CommonModifier(1,    // lifeChange
                              1,    // relativeStartSize
                              1);   // relativeEndSize
      ps->addModifier (cm);

      vm = new VortexModifier (Vec3f(300.0f, 0.0f, 0),
                                3.0f,   // strength
                                0.01f,    // damping
                                1000,     // radius
                                -30 * 3.14f / 180.0f); // angle
      ps->addModifier (vm);

      vm = new VortexModifier (Vec3f(-300.0f, -300.0f, 0),
                                4.4f,   // strength
                                0.01f,    // damping
                                500,     // radius
                                -30 * 3.14f / 180.0f); // angle
      ps->addModifier (vm);

      return ps;
    case 4:
      ps = new ParticleSystem("../Media/Images/smoke.png");

      te = new AreaEmitter (100000,
                            Vec3f(0, 0, 0),                //position
                            20, //particlesPerFrame,
  						              100, //width
  						              100, //height 
							              50, // minParticleSize
							              50, // maxParticleSize
							              0.1f, // minParticleVelocity
							              0.1f);  // maxParticleVelocity
      ps->addEmitter (te);

      fm = new FluidModifier(this, te, 100, Vec3f::zero(), 1800, 1000);
      ps->addModifier (fm);

      te = new AreaEmitter (100000,
                            Vec3f(0, 0, 0),                //position
                            100, //particlesPerFrame,
  						              1800, //width
  						              1000, //height 
							              3, // minParticleSize
							              10, // maxParticleSize
							              0.0f, // minParticleVelocity
							              0.0f);  // maxParticleVelocity
      ps->addEmitter (te);

      cm = new CommonModifier(0.3,    // lifeChange
                              1,    // relativeStartSize
                              1);   // relativeEndSize
      ps->addModifier (cm);

      colorModifier = new ColorModifier (ColorAf(0.1, 0.3, 1.0, 1),   // startColor
                                         ColorAf(0.1, 0.3, 1.0, 1), // middleColor
                                         ColorAf(0.0, 0.0, 0.0, 1),   // endColor
                                         0.5f);                     // middleTime

      ps->addModifier (colorModifier);

      return ps;
  }

  return NULL;
}

CINDER_APP_BASIC (ParticleApp, RendererGl)
