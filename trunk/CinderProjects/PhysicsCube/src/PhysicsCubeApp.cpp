#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "PhysicsEngine.h"
#include "DynamicObject.h"
#include "StaticObject.h"
#include "BoundingBox.h"
#include "MovingCamera.h"

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

  PhysicsEngine *physicsEngine;
  MovingCamera *camera;
};

void PhysicsCubeApp::setup()
{
  camera = new MovingCamera(300.0f);
  physicsEngine = new PhysicsEngine();
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
  physicsEngine->update();
}

void PhysicsCubeApp::draw()
{
  camera->setMatrices ();

	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
  
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

  physicsEngine->draw();
}


CINDER_APP_BASIC( PhysicsCubeApp, RendererGl )
