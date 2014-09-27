#include "cinder/app/AppBasic.h"
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

class Box2DTestApp : public AppBasic {
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
	bool mousePressed;
	Vec2f mousePos;
	Vec2f mouseVel;
};

void Box2DTestApp::setup()
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

void Box2DTestApp::mouseDown( MouseEvent event )
{
	mousePressed = true;
}

void Box2DTestApp::mouseUp( MouseEvent event )
{
	mousePressed = false;
}

void Box2DTestApp::mouseMove( MouseEvent event )
{
	mouseVel = ( event.getPos() - mousePos );
	mousePos = event.getPos();
}

void Box2DTestApp::mouseDrag( MouseEvent event )
{
	mouseMove( event );
}

void Box2DTestApp::keyDown(KeyEvent event) {
	switch(event.getChar()) {
		case ' ': particleController.removeAll(); break;
	}
}

void Box2DTestApp::update()
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

void Box2DTestApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) );
	gl::enableAlphaBlending();

	particleController.draw();
}

CINDER_APP_BASIC( Box2DTestApp, RendererGl )
