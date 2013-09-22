#include "Vessel.h"

#include "cinder/gl/gl.h"
#include "cinder/app/App.h"
#include "cinder/ObjLoader.h"

#include <Box2D/Box2d.h>

#include "ParticleSystemHelper.h"
#include "Emitter.h"
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
: m_isDead        (false),
  m_moveCapForce  (vesselDef.moveCapForce),
  m_moveDistConst (vesselDef.moveDistConst),
  m_leanConst     (vesselDef.leanConst),
  m_life          (vesselDef.initialLife)
{
  // Set model
  m_model = ModelLibrary::getSingleton ().getModel (vesselDef.modelName);

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

  // create collision shape
  float xMin, xMax, yMin, yMax;
	b2PolygonShape boxShape;

  m_model->getMinMax (xMin, xMax, yMin, yMax);
  float halfW = (xMax-xMin)/2.f;
  float halfH = (yMax-yMin)/2.f;
  float shrinkFactor = 0.8f;
	boxShape.SetAsBox (shrinkFactor * Conversions::toPhysics (halfW), 
                     shrinkFactor * Conversions::toPhysics (halfH), 
                     shrinkFactor * Conversions::toPhysics (Vec2f (xMax-halfW, yMax-halfH)), 
                     0.f);

  // create fixture
	b2FixtureDef fixtureDef;
  fixtureDef.shape                = &boxShape;
	fixtureDef.density              = vesselDef.fixtureDensity;
	fixtureDef.friction             = 0.3f;
	fixtureDef.restitution          = 0.f; // bounce
  fixtureDef.filter.categoryBits  = vesselDef.category;

  if (vesselDef.category == EntityCategory_Enemies_E)
    fixtureDef.filter.maskBits = EntityCategory_OurShip_E | EntityCategory_OurShots_E | EntityCategory_Enemies_E | EntityCategory_EnemySwarm_E;
  else if (vesselDef.category == EntityCategory_EnemySwarm_E)
    fixtureDef.filter.maskBits = EntityCategory_OurShip_E | EntityCategory_OurShots_E | EntityCategory_Enemies_E;
  else if (vesselDef.category == EntityCategory_OurShip_E)
    fixtureDef.filter.maskBits = EntityCategory_Enemies_E | EntityCategory_EnemyShots_E | EntityCategory_EnemySwarm_E;

	m_body->CreateFixture (&fixtureDef);
}

Vessel::~Vessel ()
{
  World::getPhysicsWorld ().DestroyBody (m_body);
}

void Vessel::update (const float dt, const PositionAndAngle& positionAndAngle)
{
  //------------ Handle position ---------------------
  b2Vec2 distance = Conversions::toPhysics (positionAndAngle.m_position) - m_body->GetPosition ();
  b2Vec2 force = m_moveDistConst * distance;
  if (force.LengthSquared () > m_moveCapForce * m_moveCapForce)
  {
    force.Normalize ();
    force = m_moveCapForce * force;
  }
  m_body->ApplyForceToCenter (force);
  Vec2f position = Conversions::fromPhysics (m_body->GetPosition ());

  if (m_life <= 0.f)
    explode ();

#if _DEBUG
  m_previousDesiredPosition = positionAndAngle.m_position;
#endif

  //------------ Handle rotation ---------------------
  const float rotationConst = 1.f;
  float angleDifferance = positionAndAngle.m_angle-m_body->GetAngle ();

  if (angleDifferance > toRadians (180.f))
    angleDifferance -= toRadians (360.f);
  else if (angleDifferance < toRadians (-180.f))
    angleDifferance += toRadians (360.f);

  m_body->ApplyAngularImpulse (rotationConst * angleDifferance);
}

void Vessel::draw ()
{
  const float leanConst = m_leanConst;

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
  gl::drawSolidCircle (m_previousDesiredPosition, 10);
  gl::enable (GL_LIGHTING);
#endif
}


void Vessel::explode ()
{
  ParticleSystemHelper::createExplosion (Conversions::fromPhysics3f (m_body->GetPosition ()),
                                         Conversions::fromPhysics3f (m_body->GetLinearVelocity ()) / 60.f);

  eliminate ();
}

//----------------------------------------------------------------------------------------------------------------------