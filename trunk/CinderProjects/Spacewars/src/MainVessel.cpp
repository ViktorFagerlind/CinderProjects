#include "MainVessel.h"

#include "cinder/gl/gl.h"
#include "cinder/app/App.h"
#include "cinder/ObjLoader.h"

#include <Box2D/Box2d.h>

#include "ParticleSystemManager.h"
#include "ModelLibrary.h"
#include "AreaEmitter.h"
#include "Macros.h"
#include "PointEmitter.h"
#include "CommonModifier.h"
#include "ColorModifier.h"
#include "Conversions.h"
#include "World.h"

using namespace ci;
using namespace ci::app;

//----------------------------------------------------------------------------------------------------------------------

Engine::Engine ()
{
  //------------- base fire particle system ---------------------
  ParticleSystem *particleSystem = new ParticleSystem (loadImage (LOAD_IMAGE_FILE_OR_RESOURCE("particle1.jpg")));

  m_baseEmitter = new AreaEmitter (100,                  // maxNofParticles,
                                     Vec3f(app::getWindowWidth()/2.f, app::getWindowHeight()/2.f,0),         // position, 
  						                       2,                  // particlesPerFrame, 
  						                       7,                  // width
  						                       7,                  // height 
                                     7,                  // depth,
							                       8,                  // minParticleSize,
							                       12,                 // maxParticleSize,
							                       Vec3f (0, -6, 0),   // baseVelocity,
							                       0.5f);              // randVelocity
  particleSystem->addEmitter  (m_baseEmitter);

  CommonModifier *commonModifier = new CommonModifier (4.0f, 1.0f, 0.6f);
  ColorModifier  *colorModifier  = new ColorModifier  (ColorAf(1.0f,  0.8f,  0.2f,  1.0f),  //startColor 
                                                       ColorAf(1.0f,  0.5f,  0.2f,  0.7f),  //middleColor
                                                       ColorAf(1.0f,  0.2f,  0.2f,  0.0f),  //endColor
                                                       0.5f);                               //float middleTime)
  particleSystem->addModifier (commonModifier);
  particleSystem->addModifier (colorModifier);

  ParticleSystemManager::getSingleton().addParticleSystem (particleSystem);
  //------------- flare particle system ---------------------

  particleSystem = new ParticleSystem (loadImage (LOAD_IMAGE_FILE_OR_RESOURCE("particle1.jpg")));

  m_flareEmitter = new PointEmitter (100, 
                                     Vec3f::zero (), 
                                     2, 
                                     40, 
                                     40, 
                                     Vec3f::zero (), 
                                     0.f);
  particleSystem->addEmitter  (m_flareEmitter);

  commonModifier = new CommonModifier (20.0f, 1.0f, 0.6f);
  colorModifier  = new ColorModifier  (ColorAf (0.8f,  0.8f,  1.0f,  0.4f),  //startColor 
                                       ColorAf (0.8f,  0.8f,  1.0f,  0.2f),  //middleColor
                                       ColorAf (0.8f,  0.8f,  1.0f,  0.0f),  //endColor
                                       0.5f);                                //float middleTime)  particleSystem->addModifier (commonModifier);
  particleSystem->addModifier (commonModifier);
  particleSystem->addModifier (colorModifier);

  ParticleSystemManager::getSingleton().addParticleSystem (particleSystem);
}

Engine::~Engine ()
{
}

void Engine::update (float dt, const Vec2f& pos)
{
  Vec3f exhaustPos = Conversions::Vec2fTo3f (pos - Vec2f (0.f, 25.f));

  m_baseEmitter->setPosition  (exhaustPos + Vec3f (0.f, 0.f, 20.f));
  m_flareEmitter->setPosition (exhaustPos + Vec3f (0.f, -25.f, 20.f));
}

void Engine::draw ()
{
}

//----------------------------------------------------------------------------------------------------------------------

MainVessel::MainVessel ()
{
  // -------------- setup physics ----------------
	// create a dynamic body
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set (Conversions::toPhysics (0.f), Conversions::toPhysics (0.f));
  bodyDef.linearDamping  = 15.f;
  bodyDef.angularDamping = 15.f;

	// Set userData to point at the class if needed to be used later on...
	bodyDef.userData = this;
	m_body = World::getPhysicsWorld ().CreateBody (&bodyDef);

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox (Conversions::toPhysics (20.f), Conversions::toPhysics (60.f));

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	fixtureDef.restitution = 0.f; // bounce
  fixtureDef.filter.categoryBits = OurShip_E;
  fixtureDef.filter.maskBits     = Enemies_E | EnemyShots_E;

	m_body->CreateFixture (&fixtureDef);

  m_body->SetUserData (this);

  // -------------- create weapons ----------
  m_leftLaser.reset  (new Weapon (Vec2f (-15.f, 60.f)));
  m_rightLaser.reset (new Weapon (Vec2f ( 15.f, 60.f)));

  // Set life
  m_life = 100.f;

  // Set model
  m_model = ModelLibrary::getSingleton ().getModel ("main_vessel");
}

MainVessel::~MainVessel ()
{
  World::getPhysicsWorld ().DestroyBody (m_body);
}

void MainVessel::update (float dt, const Vec2f& touchPos)
{
  //------------ Handle movement ---------------------
  const float capDistanceSquared  = 1000.f;
  const float distConst           = 400.f;

  b2Vec2 limitedDistance = Conversions::toPhysics (touchPos) - m_body->GetPosition ();
  if (limitedDistance.LengthSquared () > capDistanceSquared)
  {
    limitedDistance.Normalize ();
    limitedDistance = sqrtf (capDistanceSquared) * limitedDistance;
  }

  b2Vec2 force = distConst * limitedDistance;

  m_body->ApplyForceToCenter (force);

  Vec2f position = Conversions::fromPhysics (m_body->GetPosition ());

  //------------ Handle rotation ---------------------
  const float rotationConst = 1.f;

  m_body->ApplyAngularImpulse (-rotationConst * m_body->GetAngle ());

  //------------ Update sub components----------------
  m_engine.update (dt, position);

  m_leftLaser->update  (dt, position);
  m_rightLaser->update (dt, position);
}

void MainVessel::draw ()
{
  const float leanConst = 0.001f;

  Vec2f rightForVehicle = Vec2f (1.f, 0.f);
  rightForVehicle.rotate (m_body->GetAngle ());

  Vec2f speed = Conversions::fromPhysics (m_body->GetLinearVelocity ());
  float rightNess = speed.dot (rightForVehicle) * leanConst;

  m_model->draw (Conversions::fromPhysics (m_body->GetPosition ()), // position
                 m_body->GetAngle (),                               // zAngle
                 rightNess);                                        // yAngle


  m_leftLaser->draw  ();
  m_rightLaser->draw ();
}

//----------------------------------------------------------------------------------------------------------------------