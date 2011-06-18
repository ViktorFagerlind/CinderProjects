#include <Windows.h>

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"

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

  Particle        *p;
  PointEmitter    *pe;
  CommonModifier  *cm;
};

void ParticleApp::setup()
{
  //p   = new Particle (Vec3f(100, 100, 0), Vec3f(0, 0, 0), 10);
  pe  = new PointEmitter(Vec3f((float)getWindowWidth()/2.0f, (float)getWindowHeight()/2.0f, 0), //position
                         "test",  // image file
                         100.0f,  // particles per frame
                         1.0f,    // min size
                         2.0f,    // max size
                         -1.0f,    // min vel
                         1.0f);   // max vel

  cm = new CommonModifier(1, 1, 0.5, 1, 1);
  pe->addModifier (cm);
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
	gl::clear( Color( 0, 0, 0 ) ); 

  pe->draw();
}

CINDER_APP_BASIC( ParticleApp, RendererGl )
