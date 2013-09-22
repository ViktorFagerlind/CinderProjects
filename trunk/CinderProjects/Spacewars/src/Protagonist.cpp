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

Engine::Engine ()
{
  m_baseEmitter  = ParticleSystemHelper::createThrustSystem ();

  m_flareEmitter = ParticleSystemHelper::createFlareSystem ();
}

Engine::~Engine ()
{
}

void Engine::update (const float dt, const Vec2f& pos)
{
  Vec3f exhaustPos = Conversions::Vec2fTo3f (pos - Vec2f (0.f, 25.f));

  m_baseEmitter->setPosition  (exhaustPos + Vec3f (0.f, 0.f, 20.f));
  m_flareEmitter->setPosition (exhaustPos + Vec3f (0.f, -25.f, 20.f));
}

void Engine::draw ()
{
}

//----------------------------------------------------------------------------------------------------------------------

Protagonist::Protagonist ()
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
  vesselDef.modelName           = "main_vessel";

  vesselDef.bodyLinearDamping   = 15.f;
  vesselDef.bodyAngularDamping  = 15.f;
  vesselDef.fixtureDensity      = 1.f;

  m_vessel.reset (new ProtagonistVessel (vesselDef));

  // -------------- create weapons ----------
  m_leftLaser.reset  (new Weapon (Vec2f (-15.f, 60.f)));
  m_rightLaser.reset (new Weapon (Vec2f ( 15.f, 60.f)));
}

Protagonist::~Protagonist ()
{
}

void Protagonist::update (const float dt, const Vec2f& touchPos)
{
  m_vessel->update (dt, PositionAndAngle (touchPos.x, touchPos.y, 0.f));

  //------------ Update sub components----------------
  Vec2f position = m_vessel->getPosition ();

  m_engine.update      (dt, position);
  m_leftLaser->update  (dt, position);
  m_rightLaser->update (dt, position);
}

void Protagonist::draw ()
{
  m_vessel->draw ();

  m_leftLaser->draw  ();
  m_rightLaser->draw ();
}

//----------------------------------------------------------------------------------------------------------------------