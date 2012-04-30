#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class BlobAppApp : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
};

void BlobAppApp::setup()
{
}

void BlobAppApp::mouseDown( MouseEvent event )
{
}

void BlobAppApp::update()
{
}

void BlobAppApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP_BASIC( BlobAppApp, RendererGl )
