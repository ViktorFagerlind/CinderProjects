#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "DynamicObject.h"
#include "StaticObject.h"
#include "BoundingBox.h"
#include "../../SkyRoads/src/MovingCamera.h"
#include "cinder/Timer.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class PhysicsCubeApp : public AppBasic {
  public:
	void setup();
	void update();
	void draw();
  void prepareSettings (Settings *settings);
  void keyDown (KeyEvent event);
  void mouseMove (MouseEvent event);

  DynamicObject *cube;
  //DynamicObject *plane;
  StaticObject *plane;
  DynamicObject *sphere;
  MovingCamera *camera;

  Timer mTimer;
  float mLastTimeStamp;
};

void PhysicsCubeApp::setup()
{
  camera = new MovingCamera(300.0f);
  cube = new DynamicObject(1.0f, Vec3f(0, 0, 0), 70.0f, 70.0f, 70.0f);
  cube->setPosition(Vec3f(300, 0, 0));

  //plane = new DynamicObject(1.0f, Vec3f(0, 0, 0), 70.0f, 70.0f);
  //plane->setPosition(Vec3f(250, 100, 10));

  plane = new StaticObject(1.0f, Vec3f(0, 0, 0), 70.0f, 70.0f, Vec3f(0, 0, 100), Vec3f(100, 0, 0), Vec3f(0, 100, 0));

  sphere = new DynamicObject(1.0f, Vec3f(0, 0, 0), 35.0f);
  sphere->setPosition(Vec3f(100, 0, 0));

  mTimer.start ();
  mLastTimeStamp = (float)mTimer.getSeconds ();
}

void PhysicsCubeApp::prepareSettings (Settings *settings)
{
  settings->setWindowSize (1024, 768);
}

void PhysicsCubeApp::keyDown (KeyEvent event)
{
  camera->keyDown (event);
}

void PhysicsCubeApp::mouseMove (MouseEvent event)
{
  camera->mouseMove (event);
}

void PhysicsCubeApp::update()
{
  // Calculate time
  float newTime = (float)mTimer.getSeconds ();
  float deltaTime = newTime - mLastTimeStamp;
  mLastTimeStamp = newTime;


  if (cube->getPosition().y > 300)
  {
    cube->applyForce(Vec3f(0, -50, 0));
    cube->applyTorque(Vec3f(0, 0, 0), Vec3f(0.0, 0.0, 0.0));
  }
  else
  {
    cube->applyForce(Vec3f(0, 30, 0));
    cube->applyTorque(Vec3f(0, 1, 0), Vec3f(100.0, 0.0, 0.0));
  }

  cube->update(deltaTime);
  plane->update(deltaTime);
  sphere->update(deltaTime);

}

void PhysicsCubeApp::draw()
{
  camera->setViewMatrix();

	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
  cube->draw();
  plane->draw();
  sphere->draw();

  //x-axis
  gl::color(1.0f, 0.1f, 0.1f);
  gl::drawVector(Vec3f(0, 0, 0), Vec3f(50, 0, 0), 10.0f, 5.0f);

  //y-axis
  gl::color(0.1f, 1.0f, 0.1f);
  gl::drawVector(Vec3f(0, 0, 0), Vec3f(0, 50, 0), 10.0f, 5.0f);

  //z-axis
  gl::color(0.3f, 0.3f, 1.0f);
  gl::drawVector(Vec3f(0, 0, 0), Vec3f(0, 0, 50), 10.0f, 5.0f);
  
  //gl::drawCoordinateFrame(100.0f, 20.0f, 10.0f);
}


CINDER_APP_BASIC( PhysicsCubeApp, RendererGl )
