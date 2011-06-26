#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"

#include "MyStrings.h"
#include "ParticleSystemManager.h"
#include "ParticleSystem.h"

#include "AreaEmitter.h"
#include "PointEmitter.h"
#include "ImageEmitter.h"

#include "CommonModifier.h"
#include "ColorModifier.h"
#include "GravityModifier.h"
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
	void update();
	void draw();

private:
	ParticleSystem* createParticleSystem(size_t index);
  
private:
  Font mFont;

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

	// clear out the window with black
	glClearColor (0, 0, 0, 0);
	glClear (GL_COLOR_BUFFER_BIT);

	glDepthMask (GL_FALSE);
	glDisable (GL_DEPTH_TEST);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE);

  mManager->draw();

  if ((frameCount % 10) == 0)
    particleCount = mManager->getCount();

  frameCount++;

  gl::drawString( "FPS: " + MyString::toString((size_t)getAverageFps()) + "Count: " + MyString::toString(particleCount), Vec2f( 10.0f, 10.0f ), Color::white(), mFont );
}

void ParticleApp::mouseDown( MouseEvent event )
{
  size_t count = mManager->getCount();
}

void ParticleApp::keyDown( KeyEvent event )
{
  char c = event.getChar();
  size_t index = (size_t) (c-'1');

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

  CommonModifier  *cm;
  ColorModifier   *colorModifier;
  GravityModifier *gm;
  VortexModifier  *vm;
  PerlinModifier  *pm;
  ParticleSystem  *ps;

  switch(index)
  {
    case 0:
      ps = new ParticleSystem();

      pe = new PointEmitter (300000,
                              Vec3f((float)getWindowWidth()/2.0f, 100, 0), //position
                              "../Media/Images/ring_flare2.png",  // image file
                              1.0f,  // particles per frame
                              10.0f,   // min size
                              20.0f,   // max size
                              -2.0f,   // min vel
                              2.0f);   // max vel
      ps->addEmitter (pe);

      cm = new CommonModifier(1,    // lifeChange
                              1,    // relativeStartSize
                              0.5);  // relativeEndSize
      ps->addModifier (cm);

      gm = new GravityModifier(Vec3f(0,0.08f,0));
      ps->addModifier (gm);

      return ps;
    case 1:
      ps = new ParticleSystem();

      ie = new 	ImageEmitter(300000,
                              "../Media/Images/fire.png", 
                              100.0f,
                              "../Media/Images/lines.png", 
                              Vec3f((float)getWindowWidth()/2.0f, 500, 0),
                              10.0f,   // min size
                              20.0f,   // max size
                              -1.0f,   // min vel
                              1.0f,    // max vel
							                500,     // emitter width
							                250,     // emitter height
							                10);     // emitter depth
      ps->addEmitter (ie);
        

      cm = new CommonModifier(1,    // lifeChange
                              1,    // relativeStartSize
                              0);  // relativeEndSize
      ps->addModifier (cm);

      gm = new GravityModifier(Vec3f(0,-0.05f,0));
      ps->addModifier (gm);

      return ps;
    case 2:
      ps = new ParticleSystem();

      ae = new AreaEmitter(300000,
                            Vec3f(30, 30, 0),                //position
                            "../Media/Images/flare.png",   // image file
                            500, //particlesPerFrame,
  						              getWindowWidth()-60, //width
  						              getWindowHeight()-60, //height 
							              5, // minParticleSize
							              5, // maxParticleSize
							              0.0f, // minParticleVelocity
							              0.0f);  // maxParticleVelocity
      ps->addEmitter (ae);

      gm = new GravityModifier(Vec3f(0,0.1f,0));
      ps->addModifier (gm);

      cm = new CommonModifier(1,    // lifeChange
                              1,    // relativeStartSize
                              2);    // relativeEndSize
      ps->addModifier (cm);

      colorModifier = new ColorModifier (ColorAf(1, 0.5, 0.2, 0), // startColor
                                          ColorAf(0.8, 0.3, 0.4, 1), // middleColor
                                          ColorAf(0.2, 0.2, 1, 0), // endColor
                                          0.5f);            // middleTime
      ps->addModifier (colorModifier);

//      pm = new PerlinModifier();
//      ps->addModifier (pm);

      vm = new VortexModifier (Vec3f(800.0f, 500.0f, 0),
                                1.0f,   // strength
                                0.01f,    // damping
                                500,     // radius
                                -30 * 3.14f / 180.0f); // angle
      ps->addModifier (vm);

      vm = new VortexModifier (Vec3f(200.0f, 300.0f, 0),
                                0.4f,   // strength
                                0.01f,    // damping
                                500,     // radius
                                -30 * 3.14f / 180.0f); // angle
      ps->addModifier (vm);

      return ps;
    case 3:
      ps = new ParticleSystem();

      te = new AreaEmitter(500000,
                            Vec3f(30, 30, 0),                //position
                            "../Media/Images/flare.png",   // image file
                            5000, //particlesPerFrame,
  						              getWindowWidth()-60, //width
  						              getWindowHeight()-60, //height 
							              1, // minParticleSize
							              1, // maxParticleSize
							              0.0f, // minParticleVelocity
							              0.0f);  // maxParticleVelocity
      ps->addEmitter (te);

      cm = new CommonModifier(1,    // lifeChange
                              1,    // relativeStartSize
                              1);   // relativeEndSize
      ps->addModifier (cm);

      vm = new VortexModifier (Vec3f(800.0f, 500.0f, 0),
                                2.0f,   // strength
                                0.001f,    // damping
                                1000,     // radius
                                -30 * 3.14f / 180.0f); // angle
      ps->addModifier (vm);

      vm = new VortexModifier (Vec3f(200.0f, 300.0f, 0),
                                3.4f,   // strength
                                0.001f,    // damping
                                500,     // radius
                                -30 * 3.14f / 180.0f); // angle
      ps->addModifier (vm);

      return ps;
  }

  return NULL;
}

CINDER_APP_BASIC (ParticleApp, RendererGl)
