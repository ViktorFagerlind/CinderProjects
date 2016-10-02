#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class iOS_TestAgainApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
    
  private:
    float angle;
    
};

void iOS_TestAgainApp::setup()
{
    angle = 0.f;
}

void iOS_TestAgainApp::mouseDown( MouseEvent event )
{
}

void iOS_TestAgainApp::update()
{
}

void iOS_TestAgainApp::draw()
{
  // clear out the window with black
  gl::clear( Color( 0, 0, 0 ) );
    
  gl::color(1.f, .5f, 0.f);
   
  gl::pushMatrices();
    
  gl::translate (vec3 (getWindowWidth()/2.f, getWindowHeight()/2.f, 0.f));
    
  gl::rotate (vec3 (angle, angle, angle));
    
  gl::drawCube (vec3 (0, 0, 0), vec3(50.f, 50.f, 50.f));
 
  gl::popMatrices();
    
  angle += 5.f;
}

CINDER_APP_NATIVE( iOS_TestAgainApp, RendererGl )
