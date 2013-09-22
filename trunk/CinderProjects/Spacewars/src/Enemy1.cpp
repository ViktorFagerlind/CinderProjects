#include "Enemy1.h"

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
using namespace std;
using namespace ci::app;

//----------------------------------------------------------------------------------------------------------------------

Enemy1::Enemy1 ()
{
  // -------------- initial position ----------
  m_positionAndAngle = PositionAndAngle (-300, 700, toRadians (180.f));

  // -------------- setup vessel ----------
  // create collision shape
	b2PolygonShape boxShape;
	boxShape.SetAsBox (Conversions::toPhysics (20.f), Conversions::toPhysics (60.f));

  // define vessel
  VesselDef vesselDef;
  vesselDef.position            = m_positionAndAngle.value ().m_position;
  vesselDef.angle               = m_positionAndAngle.value ().m_angle;
  vesselDef.isEnemy             = true;
  vesselDef.initialLife         = 100.f;
  vesselDef.moveCapForce        = 400.f;
  vesselDef.moveDistConst       = 150.f;
  vesselDef.leanConst           = .003f;
  vesselDef.modelName           = "enemy_vessel";

  vesselDef.bodyLinearDamping   = 10.f;
  vesselDef.bodyAngularDamping  = 15.f;
  vesselDef.fixtureShape        = &boxShape;
  vesselDef.fixtureDensity      = 1.f;

  m_vessel.reset (new Vessel (vesselDef));

  // -------------- setup animation ----------
  timeline().appendTo (&m_positionAndAngle, PositionAndAngle (-300,-400, toRadians (180.f)), 2.0f, EaseNone());
	timeline().appendTo (&m_positionAndAngle, PositionAndAngle (   0, 200, toRadians (220.f)), 2.5f, EaseNone());
	timeline().appendTo (&m_positionAndAngle, PositionAndAngle (  50, 200, toRadians (180.f)), 0.5f, EaseNone());
	timeline().appendTo (&m_positionAndAngle, PositionAndAngle (  50,-300, toRadians (180.f)), 1.0f, EaseNone());
	timeline().appendTo (&m_positionAndAngle, PositionAndAngle ( 400,   0, toRadians (300.f)), 1.5f, EaseNone());
  timeline().appendTo (&m_positionAndAngle, PositionAndAngle ( 300, 800, toRadians (360.f)), 2.0f, EaseNone())
    .finishFn (bind (&Vessel::eliminate, m_vessel));
}

Enemy1::~Enemy1 ()
{
}

void Enemy1::update (const float dt)
{
  m_vessel->update (dt, m_positionAndAngle);
}

void Enemy1::draw ()
{
  m_vessel->draw ();
}

//----------------------------------------------------------------------------------------------------------------------