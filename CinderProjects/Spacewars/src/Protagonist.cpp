#include "Protagonist.h"

#include "cinder/gl/gl.h"
#include "cinder/app/App.h"
#include "cinder/ObjLoader.h"

#include <Box2D/Box2d.h>

#include "PositionAndAngle.h"
#include "Conversions.h"
#include "World.h"

#include "ParticleSystemHelper.h"

#include "Emitter.h"

using namespace ci;
using namespace ci::app;

//----------------------------------------------------------------------------------------------------------------------

Engine::Engine (const Vec3f& relativePos)
: m_relativePos (relativePos)
{
  m_baseEmitter  = ParticleSystemHelper::createThrustSystem ();
  m_flareEmitter = ParticleSystemHelper::createFlareSystem ();
}

Engine::~Engine ()
{
}

void Engine::update (const float dt, const Vessel *vessel)
{
  m_baseEmitter->setPosition  (vessel->vesselPositionToWorld (m_relativePos + Vec3f (0.f,   0.f, 20.f)));
  m_flareEmitter->setPosition (vessel->vesselPositionToWorld (m_relativePos + Vec3f (0.f, -25.f, 20.f)));

  m_baseEmitter->setRotation  (vessel->getRotation ());
  m_flareEmitter->setRotation (vessel->getRotation ());
}

void Engine::draw ()
{
}

//----------------------------------------------------------------------------------------------------------------------

Protagonist::Protagonist ()
: m_engine (Vec3f (0.f, -25.f, 0.f))
{
  // -------------- setup vessel ----------

  // define vessel
  VesselDef vesselDef;
  vesselDef.position            = Vec2f (0.f, 0.f);
  vesselDef.angle               = 0.f;
  vesselDef.category            = EntityCategory_OurShip_E;
  vesselDef.initialLife         = 500.f;
  vesselDef.moveCapForce        = 4000.f;
  vesselDef.moveDistConst       = 400.f;
  vesselDef.leanConst           = .001f;
  vesselDef.modelName           = "space_barrel";

  vesselDef.bodyLinearDamping   = 15.f;
  vesselDef.bodyAngularDamping  = 15.f;
  vesselDef.fixtureDensity      = 1.f;

  m_vessel.reset (new ProtagonistVessel (vesselDef));

  // -------------- create weapons ----------
  m_leftLaser.reset  (new Lazer (Vec3f (-15.f, 60.f, 0.f), ColorAf (.2f, .5f, 1.f)));
  m_rightLaser.reset (new Lazer (Vec3f ( 15.f, 60.f, 0.f), ColorAf (.2f, .5f, 1.f)));
}

Protagonist::~Protagonist ()
{
}

void Protagonist::update (const float dt, const Vec2f& touchPos)
{
  m_vessel->update (dt, PositionAndAngle (touchPos.x, touchPos.y, 0.f));

  //------------ Update sub components----------------
  m_engine.update      (dt, m_vessel.get ());
  m_leftLaser->update  (dt, m_vessel.get ());
  m_rightLaser->update (dt, m_vessel.get ());
}

void Protagonist::drawSolid ()
{
  m_vessel->drawSolid ();

  m_leftLaser->drawSolid  ();
  m_rightLaser->drawSolid ();
}

void Protagonist::drawTransparent ()
{
  m_leftLaser->drawTransparent  ();
  m_rightLaser->drawTransparent ();
}

//----------------------------------------------------------------------------------------------------------------------