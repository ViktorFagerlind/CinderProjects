#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"

#include "MyStrings.h"
#include "ParticleSystem.h"

#include "AreaEmitter.h"
#include "PointEmitter.h"
#include "ImageEmitter.h"

#include "CommonModifier.h"
#include "GravityModifier.h"
#include "VortexModifier.h"

using namespace ci;
using namespace ci::app;

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
	void createEmitter(size_t index);
  
private:
  Font mFont;

  ParticleSystem *ps;
};


void ParticleApp::prepareSettings( Settings *settings )
{
    settings->setWindowSize (1280, 768);
    settings->setFrameRate (60.0f);
}

void ParticleApp::setup()
{
  ps = new ParticleSystem();

  mFont = Font( "Quicksand Book Regular", 12.0f );

  setFrameRate (30.0f);
}

void ParticleApp::update()
{
  ps->update();
}

void ParticleApp::draw()
{
	// clear out the window with black
	glClearColor (0, 0, 0, 0);
	glClear (GL_COLOR_BUFFER_BIT);

	glDepthMask (GL_FALSE);
	glDisable (GL_DEPTH_TEST);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE);

  ps->draw();

  gl::drawString( "Framerate: " + MyString::toString((size_t)getAverageFps()), Vec2f( 10.0f, 10.0f ), Color::white(), mFont );
}

void ParticleApp::mouseDown( MouseEvent event )
{
  size_t count = ps->getCount();
}

void ParticleApp::keyDown( KeyEvent event )
{
  char c = event.getChar();

  createEmitter((size_t) (c-'1'));
}

void ParticleApp::createEmitter(size_t index)
{
  static PointEmitter *pe = NULL;
  static AreaEmitter  *ae = NULL;
  static ImageEmitter *ie = NULL;

  CommonModifier  *cm;
  GravityModifier *gm;
  VortexModifier  *vm;

  switch(index)
  {
    case 0:
      if (pe == NULL)
      {
        pe = new PointEmitter(Vec3f((float)getWindowWidth()/2.0f, 100, 0), //position
                               "../Media/Images/ring_flare2.png",  // image file
                               100.0f,  // particles per frame
                               10.0f,   // min size
                               20.0f,   // max size
                               -2.0f,   // min vel
                               2.0f);   // max vel

        cm = new CommonModifier(1,    // lifeChange
                                1,    // relativeStartSize
                                0.5,  // relativeEndSize
                                1,    // startOpacity
                                0);   // endOpacity

        gm = new GravityModifier(Vec3f(0,0.08f,0));

        pe->addModifier (cm);
        pe->addModifier (gm);

        ps->addEmitter (pe);
      }
      else
      {
        pe->kill();
        pe = NULL;
      }
      break;
    case 1:
      if (ie == NULL)
      {
        ie = new 	ImageEmitter("../Media/Images/fire.png", 
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

        cm = new CommonModifier(1,    // lifeChange
                                1,    // relativeStartSize
                                0,  // relativeEndSize
                                0.7,  // startOpacity
                                0.0f);   // endOpacity

        gm = new GravityModifier(Vec3f(0,-0.05f,0));

        ie->addModifier (cm);
        ie->addModifier (gm);

        ps->addEmitter (ie);
      }
      else
      {
        ie->kill();
        ie = NULL;
      }
      break;
    case 2:
      if (ae == NULL)
      {
        ae = new AreaEmitter(Vec3f(0, 0, 0), //position
                             "../Media/Images/particle1.jpg",  // image file
                             200, //particlesPerFrame,
  						               getWindowWidth(), //width
  						               getWindowHeight(), //height 
							               3, // minParticleSize
							               3, // maxParticleSize
							               0.0f, // minParticleVelocity
							               0.0f);  // maxParticleVelocity

        gm = new GravityModifier(Vec3f(0,0.05f,0));

        cm = new CommonModifier(0.7,  // lifeChange
                                1,    // relativeStartSize
                                0.5,  // relativeEndSize
                                1,    // startOpacity
                                0);   // endOpacity
        vm = new VortexModifier (Vec3f((float)getWindowWidth()/2.0f, 500.0f, 0),
                                 0.4f,   // strength
                                 0.001f,    // damping
                                 500,     // radius
                                 -30 * 3.14f / 180.0f); // angle
        ae->addModifier (vm);

        vm = new VortexModifier (Vec3f(200.0f, 300.0f, 0),
                                 0.4f,   // strength
                                 0.001f,    // damping
                                 500,     // radius
                                 -30 * 3.14f / 180.0f); // angle
        ae->addModifier (vm);

        ae->addModifier (cm);
        ae->addModifier (gm);

        ps->addEmitter (ae);
      }
      else
      {
        ae->kill();
        ae = NULL;
      }
      break;

  }
}

CINDER_APP_BASIC (ParticleApp, RendererGl)
