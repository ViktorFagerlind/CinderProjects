#include <Windows.h>

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"

#include "MyStrings.h"
#include "PointEmitter.h"
#include "CommonModifier.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class ParticleApp : public AppBasic 
{
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();

  PointEmitter    *pe;
  CommonModifier  *cm;

  Font mFont;
};

void ParticleApp::setup()
{
  pe  = new PointEmitter(Vec3f((float)getWindowWidth()/2.0f, (float)getWindowHeight()/2.0f, 0), //position
                         "test",  // image file
                         100.0f,  // particles per frame
                         1.0f,    // min size
                         2.0f,    // max size
                         -1.0f,   // min vel
                         1.0f);   // max vel

  cm = new CommonModifier(1, 1, 0.5, 1, 1);
  pe->addModifier (cm);

  mFont = Font( "Quicksand Book Regular", 12.0f );
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
//	gl::clear( Color( 0, 0, 0 ) ); 
	glClearColor( 0, 0, 0, 0 );
	glClear( GL_COLOR_BUFFER_BIT );

  pe->draw();

  gl::drawString( "Framerate: " + MyString::toString(getAverageFps()), Vec2f( 10.0f, 10.0f ), Color::white(), mFont );
}

CINDER_APP_BASIC( ParticleApp, RendererGl )
