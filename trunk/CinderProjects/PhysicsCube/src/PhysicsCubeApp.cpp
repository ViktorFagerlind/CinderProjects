#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "DynamicObject.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class PhysicsCubeApp : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();

  DynamicObject *cube;
};

void PhysicsCubeApp::setup()
{
  cube = new DynamicObject(1.0f, Matrix44f::identity(), Vec3f(0, 0, 0));
  cube->setPosition(Vec3f(150, 60, 0));
  //cube->setVelocity(Vec3f(0, 10, 0));
}

void PhysicsCubeApp::mouseDown( MouseEvent event )
{
}

void PhysicsCubeApp::update()
{
  if (cube->getPosition().y > 200)
  {
    cube->applyForce(Vec3f(0, -50, 0));
    cube->applyTorque(Vec3f(1, 0, 0), Vec3f(0.0, 5.1, 0.0));
    cube->update();
  }
  else
  {
    cube->applyForce(Vec3f(0, 20, 0));
    cube->applyTorque(Vec3f(1, 0, 0), Vec3f(0.0, 10.1, 10.0));
    cube->update();
  }

}

void PhysicsCubeApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
  cube->draw();

}

CINDER_APP_BASIC( PhysicsCubeApp, RendererGl )
