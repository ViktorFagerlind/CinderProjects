#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class VegetationAppApp : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
};

void VegetationAppApp::setup()
{
}

void VegetationAppApp::mouseDown( MouseEvent event )
{
}

void VegetationAppApp::update()
{
}

void VegetationAppApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP_BASIC( VegetationAppApp, RendererGl )
