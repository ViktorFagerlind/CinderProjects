#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include <Box2D/Box2d.h>
#include "Conversions.h"
#include "ParticleController.h"


using namespace ci;
using namespace ci::app;
using namespace std;
using namespace particles;

b2Vec2              gravity(0.0f, 10.0f);
b2World             world(gravity);
ParticleController  particleController;

class PhysicsBox2dApp : public AppNative {
  public:
	void setup();
	void mouseDown(MouseEvent event);	
	void mouseUp(MouseEvent event);
	void mouseMove(MouseEvent event);
	void mouseDrag(MouseEvent event);
	void keyDown(KeyEvent event);
	void update();
	void draw();

  private:
	float width;
	float height;
	bool  mousePressed;
	Vec2f mousePos;
	Vec2f mouseVel;
};

void PhysicsBox2dApp::setup()
{
	mousePressed = false;

	// first define a ground box (no mass)
	// 1. define a body
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(Conversions::toPhysics(app::getWindowWidth()/2), Conversions::toPhysics(app::getWindowHeight())); // pos of ground

	// 2. use world to create body
	b2Body* groundBody = world.CreateBody(&groundBodyDef);

	// 3. define fixture
	b2PolygonShape groundBox;
	groundBox.SetAsBox(Conversions::toPhysics(app::getWindowWidth()/2), Conversions::toPhysics(1.0f)); // size the ground

	// 4. create fixture on body
	groundBody->CreateFixture(&groundBox, 0.0f);

	// pass world to ParticleController
	particleController.setup(world);
}

void PhysicsBox2dApp::mouseDown( MouseEvent event )
{
	mousePressed = true;
}

void PhysicsBox2dApp::mouseUp( MouseEvent event )
{
	mousePressed = false;
}

void PhysicsBox2dApp::mouseMove( MouseEvent event )
{
	mouseVel = ( event.getPos() - mousePos );
	mousePos = event.getPos();
}

void PhysicsBox2dApp::mouseDrag( MouseEvent event )
{
	mouseMove( event );
}

void PhysicsBox2dApp::keyDown(KeyEvent event) {
	switch(event.getChar()) {
		case ' ': particleController.removeAll(); break;
	}
}

void PhysicsBox2dApp::update()
{
	if (mousePressed)
		particleController.addParticle(mousePos);
	particleController.update();

	// step physics world
	float32 timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;
	world.Step(timeStep, velocityIterations, positionIterations);
}

void PhysicsBox2dApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) );
	gl::enableAlphaBlending();

	particleController.draw();
}


CINDER_APP_NATIVE( PhysicsBox2dApp, RendererGl )
