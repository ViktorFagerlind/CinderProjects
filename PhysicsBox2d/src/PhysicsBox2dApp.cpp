#include "cinder/app/AppNative.h"
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"

#include <Box2D/Box2d.h>
#include "Conversions.h"
#include "BoxParticleController.h"

//#include "MovingCamera.h"
#include "ParticleSystemManager.h"
#include "ParticleSystem.h"
#include "CommonModifier.h"
#include "ColorModifier.h"
#include "AreaEmitter.h"
#include "PointEmitter.h"

#if defined (CINDER_COCOA_TOUCH)
  #include "cinder/MotionManager.h"
#endif


using namespace ci;
using namespace ci::app;
using namespace std;

b2Vec2                  gravity (0.0f, 10.0f);
b2World                 world (gravity);
BoxParticleController   particleController;

class Bucket;

class PhysicsBox2dApp : public AppNative {
public:
  void prepareSettings (Settings *settings);
	void setup();

	void mouseDown (MouseEvent event);	
	void mouseUp   (MouseEvent event);
	void mouseMove (MouseEvent event);
	void mouseDrag (MouseEvent event);

	void touchesBegan (TouchEvent event);
	void touchesMoved (TouchEvent event);
	void touchesEnded (TouchEvent event);

	void keyDown(KeyEvent event);
	void update();
	void draw();

  //vec2 mouseToWorld (int mouseX, int mouseY);

private:
	float m_width;
	float m_height;

	bool  m_pressed;
	vec2 m_position;
	vec2 m_velocity;

  unique_ptr<Bucket> m_bucket;

  AreaEmitter *mHeadEmitter;

//  shared_ptr<MovingCamera> mCamera;
};

class Bucket
{
public:
  Bucket ()
  : m_pos    (app::getWindowWidth()/2.f, app::getWindowHeight()*0.9f),
    m_width  (app::getWindowWidth()  * 0.5f),
    m_height (app::getWindowHeight() * 0.4f)
  {
    float w     = Conversions::toPhysics (m_width);
    float h     = Conversions::toPhysics (m_height);
    float thin  = Conversions::toPhysics (1.f);
    
	  // first define a ground box (no mass)
	  // 1. define a body
	  b2BodyDef groundBodyDef;
	  groundBodyDef.position.Set (Conversions::toPhysics (m_pos.x), Conversions::toPhysics (m_pos.y));

	  // 2. use world to create body
	  b2Body* bucketBody = world.CreateBody(&groundBodyDef);

	  // Fix bucket
	  b2PolygonShape box;
    // floor
	  box.SetAsBox (w, thin);             
	  bucketBody->CreateFixture (&box, 0.0f);
    // left side
	  box.SetAsBox  (thin, 
                   h, 
                   b2Vec2 (-w/2.f, 0.f), 
                   0.f);
	  bucketBody->CreateFixture(&box, 0.0f);
    // right side
	  box.SetAsBox  (thin, 
                   h, 
                   b2Vec2 (w/2.f, 0.f), 
                   0.f);
	  bucketBody->CreateFixture(&box, 0.0f);
  }

  void draw ()
  {
    float thin_half = 2.f;

    gl::color (.5f, .5f, .5f);

    gl::drawSolidRect (Rectf (m_pos.x - m_width/2.f - thin_half, m_pos.y - thin_half, 
                              m_pos.x + m_width/2.f + thin_half, m_pos.y + thin_half));

    gl::drawSolidRect (Rectf (m_pos.x - m_width/2.f - thin_half, m_pos.y - m_height, 
                              m_pos.x - m_width/2.f + thin_half, m_pos.y));

    gl::drawSolidRect (Rectf (m_pos.x + m_width/2.f - thin_half, m_pos.y - m_height, 
                              m_pos.x + m_width/2.f + thin_half, m_pos.y));
  }

private:
  vec2 m_pos;
  float m_width;
  float m_height;
};

void PhysicsBox2dApp::prepareSettings (Settings *settings)
{
#if !defined (CINDER_COCOA_TOUCH)
  settings->setWindowSize (1920, 1080);
#endif
}

void PhysicsBox2dApp::setup()
{
	m_pressed = false;

//  mCamera.reset (new MovingCamera (1000.f, 20.f));

	// pass world to ParticleController
	particleController.setup(world);

  m_bucket.reset (new Bucket);

#if defined (CINDER_COCOA_TOUCH)
	console() << "gyro available: " << MotionManager::isGyroAvailable() << std::endl;
	MotionManager::enable (60.0f);
#endif

  // Particle system
#if defined (CINDER_COCOA_TOUCH)
  ParticleSystem *particleSystem = new ParticleSystem (loadImage (loadResource ("basic particle 1.png")));
#else
  ParticleSystem *particleSystem = new ParticleSystem ("../Media/Images/basic particle 1.png");
#endif

  mHeadEmitter = new AreaEmitter (100,                  // maxNofParticles,
                                   vec3(app::getWindowWidth()/2.f, app::getWindowHeight()/2.f,0),         // position, 
  						                     1,                   // particlesPerFrame, 
  						                     20,                  // width
  						                     20,                  // height 
                                   20,                  // depth,
							                     4,                   // minParticleSize,
							                     6,                   // maxParticleSize,
							                     vec3 (0, 0, 0),     // baseVelocity,
							                     4.0f);               // randVelocity

  CommonModifier *commonModifier = new CommonModifier (2.0f, 1.0f, 5.0f);
  ColorModifier  *colorModifier  = new ColorModifier  (ColorAf(1.0f,  0.8f,  0.2f,  1.0f),  //startColor 
                                                       ColorAf(1.0f,  0.5f,  0.2f,  0.5f),  //middleColor
                                                       ColorAf(1.0f,  0.2f,  0.2f,  0.0f),  //endColor
                                                       0.8f);                               //float middleTime)
  particleSystem->addModifier (commonModifier);
  particleSystem->addModifier (colorModifier);
  particleSystem->addEmitter  (mHeadEmitter);

  ParticleSystemManager::getSingleton().addParticleSystem (particleSystem);
}

void PhysicsBox2dApp::mouseDown( MouseEvent event )
{
	m_pressed = true;
}

void PhysicsBox2dApp::mouseUp( MouseEvent event )
{
	m_pressed = false;
}

void PhysicsBox2dApp::mouseMove( MouseEvent event )
{
	m_velocity = event.getPos() - m_position;
	m_position = event.getPos();
}

void PhysicsBox2dApp::mouseDrag( MouseEvent event )
{
	mouseMove (event);
}

void PhysicsBox2dApp::touchesBegan( TouchEvent event )
{
  console() << "Began: " << event << std::endl;

  // Remove all particles if more than one touche
  if (getActiveTouches ().size () > 1)
    particleController.removeAll ();

	m_pressed = true;

  vector<TouchEvent::Touch>::const_iterator firstTouch = event.getTouches().begin();
	m_position = firstTouch->getPos ();
  m_velocity = vec2 (0,0);
}


void PhysicsBox2dApp::touchesMoved( TouchEvent event )
{
  console() << "Moved: " << event << std::endl;

  vector<TouchEvent::Touch>::const_iterator firstTouch = event.getTouches().begin();

  m_velocity = firstTouch->getPos () - m_position;
	m_position = firstTouch->getPos ();
}

void PhysicsBox2dApp::touchesEnded( TouchEvent event )
{
  console() << "Ended: " << event << std::endl;

	m_pressed = getActiveTouches ().size () > 0;
}


void PhysicsBox2dApp::keyDown(KeyEvent event) {

	switch(event.getChar()) 
  {
		case ' ': particleController.removeAll(); break;
	}

//  mCamera->keyDown (event);
}

/*
vec2 PhysicsBox2dApp::mouseToWorld (int mouseX, int mouseY)
{
  return vec2 ((float)mouseX  / (float)getWindowWidth ()  * 2000.f - 1000.f,
                (float)-mouseY / (float)getWindowHeight () * 1000.f + 500.f);
}
*/

void PhysicsBox2dApp::update()
{
	if (m_pressed)
		particleController.addParticle(m_position);

	particleController.update();

	// step physics world
	float32 timeStep = 1.0f / getFrameRate ();
	int32 velocityIterations = 6;
	int32 positionIterations = 2;
	world.Step (timeStep, velocityIterations, positionIterations);

  mHeadEmitter->setPosition (vec3 (m_position.x, m_position.y, 0.f));

  ParticleSystemManager::getSingleton().update ();
}

void PhysicsBox2dApp::draw()
{
	gl::clear (Color (0, 0, 0));	

  // Setup camera
  // mCamera->setMatrices ();

	particleController.draw ();

  // Get the gravity.
#if defined (CINDER_COCOA_TOUCH)
  vec3 gravity3dDirection = MotionManager::getGravityDirection ();
  gravity3dDirection.x = -gravity3dDirection.x;
  gravity3dDirection.invert ();
  vec2 gravity2dDirection = vec2 (gravity3dDirection.x, gravity3dDirection.y).normalized();		
#else
  vec2 gravity2dDirection = vec2 (0.f, 1.f);		
#endif
  
  world.SetGravity (10.f * b2Vec2 (gravity2dDirection.x, gravity2dDirection.y));
  
  vec3 center = vec3 (getWindowWidth()/2.f, getWindowHeight()/2.f, 0.f);

#if defined (CINDER_COCOA_TOUCH)
  gl::color (1, 0, 0);
  gl::drawVector (center, center + 100.f * gravity3dDirection, 20.f, 10.f);

  gl::color (0, 1, 0);
  gl::drawVector (center, center + 100.f * vec3 (gravity2dDirection.x, gravity2dDirection.y, 0.f), 20.f, 10.f);
#endif

  m_bucket->draw ();

  // Draw particle system
	gl::enableAlphaBlending ();
	gl::disableDepthWrite ();
	gl::enableAdditiveBlending ();

  ParticleSystemManager::getSingleton().draw ();

	gl::enableDepthWrite (); 
	gl::disableAlphaBlending ();
}


#if defined (CINDER_COCOA_TOUCH)
CINDER_APP_NATIVE (PhysicsBox2dApp, RendererGl(0))
#else
CINDER_APP_NATIVE (PhysicsBox2dApp, RendererGl)
#endif
