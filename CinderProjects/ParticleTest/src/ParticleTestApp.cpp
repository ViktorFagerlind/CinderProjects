#include <Windows.h>

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"

#include "Particle.h"
//#include "Emitter.h"

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

  Particle *p;
};

void ParticleApp::setup()
{
  p = new Particle (Vec3f(100, 100, 0), Vec3f(0, 0, 0), 10);
}

void ParticleApp::mouseDown( MouseEvent event )
{
}

void ParticleApp::update()
{
  p->update();
}

void ParticleApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 

 	glBegin(GL_QUADS);
  p->draw();
 	glEnd();
}

CINDER_APP_BASIC( ParticleApp, RendererGl )
