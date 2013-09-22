#include "EnemyBot.h"

#include "cinder/gl/gl.h"
#include "cinder/app/App.h"
#include "cinder/ObjLoader.h"
#include "cinder/Rand.h"

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
using namespace std;
using namespace ci::app;

//----------------------------------------------------------------------------------------------------------------------

EnemyBot::EnemyBot ()
{
  // -------------- initial position ----------
  m_positionAndAngle = PositionAndAngle (0, 800, toRadians (0.f));

  // -------------- setup vessel ----------
  // define vessel
  VesselDef vesselDef;
  vesselDef.position            = m_positionAndAngle.value ().m_position;
  vesselDef.angle               = m_positionAndAngle.value ().m_angle;
  vesselDef.category            = EntityCategory_EnemySwarm_E;
  vesselDef.initialLife         = 5.f;
  vesselDef.moveCapForce        = 200.f;
  vesselDef.moveDistConst       = 50.f;
  vesselDef.leanConst           = .003f;
  vesselDef.modelName           = "enemy_bot";

  vesselDef.bodyLinearDamping   = 10.f;
  vesselDef.bodyAngularDamping  = 15.f;
  vesselDef.fixtureDensity      = 2.5f;

  m_vessel.reset (new EnemyBotVessel (vesselDef));

  // -------------- setup animation ----------
  Vec2f prevPrevPosition = m_positionAndAngle.value ().m_position + Vec2f (0.f,10.f);
  Vec2f prevPosition     = m_positionAndAngle.value ().m_position;
  for (uint32_t i=0; i<10; i++)
  {
    Vec2f newPosition = Vec2f (Rand::randFloat (-400.f, 400.f), Rand::randFloat (-700.f, 600.f));
    Vec2f delta       = newPosition - prevPosition;
    float angle       = -acos (Vec2f(0,1).dot (delta) / delta.length ());

    timeline().appendTo (&m_positionAndAngle, PositionAndAngle (newPosition.x, newPosition.y, angle), 1.0f, EaseNone());

    prevPrevPosition  = prevPosition;
    prevPosition      = newPosition;
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

void EnemyBot::draw ()
{
  m_vessel->draw ();
}

//----------------------------------------------------------------------------------------------------------------------