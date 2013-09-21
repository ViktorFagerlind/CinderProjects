#include "Vessel.h"

#include "cinder/gl/gl.h"
#include "cinder/app/App.h"
#include "cinder/ObjLoader.h"

#include <Box2D/Box2d.h>

#include "PositionAndAngle.h"
#include "ModelLibrary.h"
#include "Macros.h"
#include "Conversions.h"
#include "World.h"

using namespace ci;
using namespace std;
using namespace ci::app;


//----------------------------------------------------------------------------------------------------------------------

Vessel::Vessel (const VesselDef& vesselDef)
: m_isDead (false)
{
  // -------------- setup physics ----------------
	// create a dynamic body
	b2BodyDef bodyDef;
	bodyDef.type            = b2_dynamicBody;
  bodyDef.position        = Conversions::toPhysics (vesselDef.position);
  bodyDef.angle           = vesselDef.angle;
  bodyDef.linearDamping   = vesselDef.bodyLinearDamping;
  bodyDef.angularDamping  = vesselDef.bodyAngularDamping;
	bodyDef.userData        = this;
	m_body = World::getPhysicsWorld ().CreateBody (&bodyDef);

  // create fixture
	b2FixtureDef fixtureDef;
  fixtureDef.shape                = vesselDef.fixtureShape;
	fixtureDef.density              = vesselDef.fixtureDensity;
	fixtureDef.friction             = 0.3f;
	fixtureDef.restitution          = 0.f; // bounce
  fixtureDef.filter.categoryBits  = vesselDef.isEnemy ? EntityCategory_Enemies_E : EntityCategory_OurShip_E;
  fixtureDef.filter.maskBits      = vesselDef.isEnemy ? 
                                      EntityCategory_OurShip_E | EntityCategory_OurShots_E | EntityCategory_Enemies_E :
                                      EntityCategory_Enemies_E | EntityCategory_EnemyShots_E;
	m_body->CreateFixture (&fixtureDef);

  // Set life
  m_life = vesselDef.initialLife;

  // Set model
  m_model = ModelLibrary::getSingleton ().getModel (vesselDef.modelName);
}

Vessel::~Vessel ()
{
  World::getPhysicsWorld ().DestroyBody (m_body);
}

void Vessel::update (const float dt, const PositionAndAngle& positionAndAngle)
{
  //------------ Handle position ---------------------
  const float capForce  = 400.f;
  const float distConst = 150.f;

  b2Vec2 distance = Conversions::toPhysics (positionAndAngle.m_position) - m_body->GetPosition ();
  b2Vec2 force = distConst * distance;
  if (force.LengthSquared () > capForce * capForce)
  {
    force.Normalize ();
    force = capForce * force;
  }
  m_body->ApplyForceToCenter (force);
  Vec2f position = Conversions::fromPhysics (m_body->GetPosition ());

#if _DEBUG
  m_previousPositionAndAngle = &positionAndAngle;
#endif

  //------------ Handle rotation ---------------------
  const float rotationConst = 1.f;
  m_body->ApplyAngularImpulse (-rotationConst * (m_body->GetAngle () - positionAndAngle.m_angle));
}

void Vessel::draw ()
{
  const float leanConst = 0.003f;

  Vec2f rightForVehicle = Vec2f (1.f, 0.f);
  rightForVehicle.rotate (m_body->GetAngle ());

  Vec2f speed = Conversions::fromPhysics (m_body->GetLinearVelocity ());
  float rightNess = speed.dot (rightForVehicle) * leanConst;

  m_model->draw (Conversions::fromPhysics (m_body->GetPosition ()), // position
                 m_body->GetAngle (),                               // zAngle
                 rightNess);                                        // yAngle


#if _DEBUG
  gl::disable (GL_LIGHTING);
  gl::color (1,0,0);
  gl::drawSolidCircle (m_previousPositionAndAngle->m_position, 10);
  gl::enable (GL_LIGHTING);
#endif
}

//----------------------------------------------------------------------------------------------------------------------