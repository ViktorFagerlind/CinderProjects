#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"

#include "MyStrings.h"
#include "PointEmitter.h"
#include "CommonModifier.h"
#include "GravityModifier.h"

using namespace ci;
using namespace ci::app;

class ParticleApp : public AppBasic 
{
  public:
  void prepareSettings( Settings *settings );
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();

  PointEmitter    *pe;

  Font mFont;
};


void ParticleApp::prepareSettings( Settings *settings )
{
    settings->setWindowSize (1280, 768);
    settings->setFrameRate (60.0f);
}

void ParticleApp::setup()
{
  pe  = new PointEmitter(Vec3f((float)getWindowWidth()/2.0f, 100, 0), //position
                         "../Media/Images/ring_flare2.png",  // image file
                         500.0f,  // particles per frame
                         10.0f,    // min size
                         20.0f,    // max size
                         -2.0f,   // min vel
                         2.0f);   // max vel

  CommonModifier  *cm = new CommonModifier(1,    // lifeChange
                                            1,    // relativeStartSize
                                            0.5,  // relativeEndSize
                                            1,    // startOpacity
                                            0);   // endOpacity

  GravityModifier *gm = new GravityModifier(Vec3f(0,0.08,0));

  pe->addModifier (cm);
  pe->addModifier (gm);

  mFont = Font( "Quicksand Book Regular", 12.0f );

  setFrameRate (30.0f);
}

void ParticleApp::mouseDown( MouseEvent event )
{
  size_t count = pe->getCount();
}

void ParticleApp::update()
{
  pe->update();
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

  pe->draw();

  gl::drawString( "Framerate: " + MyString::toString((size_t)getAverageFps()), Vec2f( 10.0f, 10.0f ), Color::white(), mFont );
}

CINDER_APP_BASIC (ParticleApp, RendererGl)
