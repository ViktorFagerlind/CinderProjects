#include "EnemyBot.h"

#include "cinder/app/App.h"
#include "cinder/Rand.h"

#include "ParticleSystemHelper.h"

using namespace ci;
using namespace std;
using namespace ci::app;

//----------------------------------------------------------------------------------------------------------------------

void EnemyBotVessel::startedDying ()
{
  ParticleSystemHelper::createFireBall (Conversions::fromPhysics3f (m_body->GetPosition ()),
                                          Conversions::fromPhysics3f (m_body->GetLinearVelocity ()) / 60.f,  // Convert to 1/60s 
                                          ColorAf(1.0f,  0.7f,  0.3f,  0.7f),
                                          ColorAf(1.0f,  0.0f,  0.0f,  0.7f),
                                          0.7f);
}

void EnemyBotVessel::died ()
{
}

//----------------------------------------------------------------------------------------------------------------------

EnemyBot::EnemyBot ()
{
  // -------------- initial position ----------
  m_positionAndAngle = PositionAndAngle (Rand::randFloat (-400.f, 400.f), 800, toRadians (0.f));

  // -------------- setup vessel ----------
  // define vessel
  VesselDef vesselDef;
  vesselDef.position            = m_positionAndAngle.value ().m_position;
  vesselDef.angle               = m_positionAndAngle.value ().m_angle;
  vesselDef.category            = EntityCategory_EnemySwarm_E;
  vesselDef.initialLife         = 5.f;
  vesselDef.timeToDie           = 0.05f;
  vesselDef.moveCapForce        = 30.f;
  vesselDef.moveDistConst       = 10.f;
  vesselDef.leanConst           = 0.002f;
  vesselDef.fixedRotation       = true;
  vesselDef.modelName           = "enemy_bot";

  vesselDef.bodyLinearDamping   = 1.f;
  vesselDef.fixtureDensity      = 3.f;

  m_vessel.reset (new EnemyBotVessel (vesselDef));

  // -------------- setup animation ----------
  for (uint32_t i=0; i<10; i++)
  {
    vec2 newPosition = vec2 (Rand::randFloat (-400.f, 400.f), Rand::randFloat (-700.f, 600.f));

    timeline().appendTo (&m_positionAndAngle, PositionAndAngle (newPosition.x, newPosition.y, 0.f), 1.4f, EaseNone());
  }
  timeline().appendTo (&m_positionAndAngle, PositionAndAngle ( 0, 1000, toRadians (360.f)), 1.0f, EaseNone())
    .finishFn (bind (&Vessel::eliminate, m_vessel));
}

EnemyBot::~EnemyBot ()
{
}

void EnemyBot::update (const float dt)
{
  m_vessel->update (dt, m_positionAndAngle);
}

void EnemyBot::drawSolid ()
{
  m_vessel->drawSolid ();
}

//----------------------------------------------------------------------------------------------------------------------