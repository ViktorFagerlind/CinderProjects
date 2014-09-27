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

VesselEmitter::VesselEmitter (shared_ptr<Emitter> emitter, const Vec3f& relativePos)
: m_emitter     (emitter),
  m_relativePos (relativePos)
{
}

//----------------------------------------------------------------------------------------------------------------------

Vessel::Vessel (const VesselDef& vesselDef)
  : m_state       (State_Living_E),
  m_moveCapForce  (vesselDef.moveCapForce),
  m_moveDistConst (vesselDef.moveDistConst),
  m_leanConst     (vesselDef.leanConst),
  m_life          (vesselDef.initialLife),
  m_timeToDie     (vesselDef.timeToDie),
  m_timeOfDeath   (0.f)
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
  bodyDef.fixedRotation   = vesselDef.fixedRotation;
	bodyDef.userData        = this;
	m_body = World::getPhysicsWorld ().CreateBody (&bodyDef);

  // create collision shape
  float xMin, xMax, yMin, yMax;
	b2PolygonShape boxShape;

  m_model->getMinMax (xMin, xMax, yMin, yMax);
  float halfW = (xMax-xMin)/2.f;
  float halfH = (yMax-yMin)/2.f;
  float shrinkFactor = 0.7f;
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

Vec3f Vessel::vesselPositionToWorld (const Vec3f& vec) const
{
  return vesselRotationToWorld (vec) + getPosition ();
}

Vec3f Vessel::vesselRotationToWorld (const Vec3f& vec) const
{
  Vec3f rotatedVec = vec;

  rotatedVec.rotateX (m_rotation.x);
  rotatedVec.rotateY (m_rotation.y);
  rotatedVec.rotateZ (m_rotation.z);
  
  return rotatedVec;
}

void Vessel::eliminate () 
{
  m_state = State_Dead_E;

  clearVesselEmitters ();
}

void Vessel::addVesselEmitter (const VesselEmitter& vesselEmitter) 
{
  m_vesselEmitters.push_back (vesselEmitter);
}

void Vessel::clearVesselEmitters ()
{
  for (uint32_t i=0; i<m_vesselEmitters.size (); i++)
  {
    VesselEmitter *ve = &m_vesselEmitters[i];
    ve->m_emitter->kill ();
  }
}


void Vessel::update (const float dt, const PositionAndAngle& positionAndAngle)
{
  switch (m_state)
  {
    case State_Living_E:
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

      #if _DEBUG
        m_previousDesiredPosition = positionAndAngle.m_position;
      #endif

      //------------ Handle rotation ---------------------
      if (!m_body->IsFixedRotation ())
      {
        const float rotationConst = 0.5f;
        float angleDifferance = positionAndAngle.m_angle-m_body->GetAngle ();

        if (angleDifferance > toRadians (180.f))
          angleDifferance -= toRadians (360.f);
        else if (angleDifferance < toRadians (-180.f))
          angleDifferance += toRadians (360.f);

        m_body->ApplyAngularImpulse (rotationConst * angleDifferance);
      }

      // Move and rotate emitters
      for (uint32_t i=0; i<m_vesselEmitters.size (); i++)
      {
        VesselEmitter *ve = &m_vesselEmitters[i];
        ve->m_emitter->setPosition (vesselPositionToWorld (ve->m_relativePos));
        ve->m_emitter->setRotation (getRotation ());
      }

      //------------ Handle potential death ---------------------
      if (m_life <= 0.f)
      {
        m_state = State_Dying_E;
        m_timeOfDeath = timeline ().getCurrentTime ();

        clearVesselEmitters ();

        startedDying ();
      }

      break;
    }

    case State_Dying_E:
      if (timeline ().getCurrentTime () >= m_timeOfDeath + m_timeToDie)
      {
        m_state = State_Dead_E;
        died ();
      }

      break;

    case State_Dead_E:
      // Do nothing
      break;
  }

  // Calculate rotation from physics object
  const float leanConst = m_leanConst;

  Vec2f rightForVehicle = Vec2f (1.f, 0.f);
  rightForVehicle.rotate (m_body->GetAngle ());

  Vec2f speed   = Conversions::fromPhysics (m_body->GetLinearVelocity ());
  m_rotation.y  = speed.dot (rightForVehicle) * leanConst;

  if (m_body->IsFixedRotation ())
    m_rotation.z = atan2 (speed.y,speed.x) + toRadians (270.f);
  else
    m_rotation.z = m_body->GetAngle ();
}

void Vessel::drawSolid ()
{
  m_model->draw (Conversions::fromPhysics (m_body->GetPosition ()), // position
                 m_rotation);                                       // rotation

#if _DEBUG
  gl::disable (GL_LIGHTING);
  gl::color (1,0,0);

  gl::drawSolidCircle (m_previousDesiredPosition, 10);

  Vec3f drawDir = Vec3f(0.f, 120.f, 0.f);
  drawDir.rotateX (m_rotation.x);
  drawDir.rotateY (m_rotation.y);
  drawDir.rotateZ (m_rotation.z);
  gl::drawVector (Conversions::fromPhysics3f (m_body->GetPosition ()),
                  Conversions::fromPhysics3f (m_body->GetPosition ()) + drawDir);

  gl::enable (GL_LIGHTING);
#endif
}

//----------------------------------------------------------------------------------------------------------------------