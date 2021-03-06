#include "EnemyArrow.h"

#include "cinder/app/App.h"

#include "ParticleSystemHelper.h"
#include "AudioLibrary.h"


using namespace ci;
using namespace std;
using namespace ci::app;

//----------------------------------------------------------------------------------------------------------------------

void EnemyArrowVessel::startedDying ()
{
  vec3 position = Conversions::fromPhysics3f (m_body->GetPosition ());
  vec3 speed    = Conversions::fromPhysics3f (m_body->GetLinearVelocity ()) / 60.f;  // Convert to 1/60s 

  ParticleSystemHelper::createFireBall (position,
                                        speed,
                                        ColorAf(0.5f,  0.8f,  0.9f,  1.0f),
                                        ColorAf(0.1f,  0.1f,  0.9f,  1.0f),
                                        2.0f);
}

void EnemyArrowVessel::died ()
{
  vec3 position = Conversions::fromPhysics3f (m_body->GetPosition ());
  vec3 speed    = Conversions::fromPhysics3f (m_body->GetLinearVelocity ()) / 60.f;  // Convert to 1/60s 

  ParticleSystemHelper::createSparkExplosion (position,
                                              speed,
                                              Color(0.5f,  0.7f, 1.0f),
                                              2.0f);
	
  AudioLibrary::getSingleton ().play (AudioClip_Explosion_E);
}

//----------------------------------------------------------------------------------------------------------------------

EnemyArrow::EnemyArrow ()
{
  // -------------- initial position ----------
  m_positionAndAngle = PositionAndAngle (-300, 700, toRadians (180.f));

  // -------------- setup vessel ----------
  // define vessel
  VesselDef vesselDef;
  vesselDef.position            = m_positionAndAngle.value ().m_position;
  vesselDef.angle               = m_positionAndAngle.value ().m_angle;
  vesselDef.category            = EntityCategory_Enemies_E;
  vesselDef.initialLife         = 100.f;
  vesselDef.timeToDie           = 0.2f;
  vesselDef.moveCapForce        = 400.f;
  vesselDef.moveDistConst       = 20.f;
  vesselDef.leanConst           = .003f;
  vesselDef.modelName           = "enemy_arrow";

  vesselDef.bodyLinearDamping   = 2.f;
  vesselDef.bodyAngularDamping  = 3.f;
  vesselDef.fixtureDensity      = 1.f;

  m_vessel.reset (new EnemyArrowVessel (vesselDef));

  // -------------- create weapons ----------
  m_lazer.reset  (new Lazer (vec3 (0.f, 60.f, 0.f), ColorAf (1.f, .8f, .4f), m_vessel, EntityCategory_EnemyShots_E, 5, true));

  // -------------- setup animation ----------
  slowFire ();
  timeline().appendTo (&m_positionAndAngle, PositionAndAngle (-300,-400, toRadians (180.f)), 2.0f, EaseNone()).finishFn (bind (&EnemyArrow::fastFire, this));
	timeline().appendTo (&m_positionAndAngle, PositionAndAngle (   0, 200, toRadians (220.f)), 2.5f, EaseNone()).finishFn (bind (&EnemyArrow::slowFire, this));
	timeline().appendTo (&m_positionAndAngle, PositionAndAngle (  50, 200, toRadians (180.f)), 0.5f, EaseNone());
	timeline().appendTo (&m_positionAndAngle, PositionAndAngle (  50,-300, toRadians (180.f)), 1.0f, EaseNone()).finishFn (bind (&EnemyArrow::fastFire, this));
	timeline().appendTo (&m_positionAndAngle, PositionAndAngle ( 400,   0, toRadians (300.f)), 1.5f, EaseNone()).finishFn (bind (&EnemyArrow::slowFire, this));
  timeline().appendTo (&m_positionAndAngle, PositionAndAngle ( 300, 800, toRadians (360.f)), 2.0f, EaseNone())
    .finishFn (bind (&Vessel::eliminate, m_vessel));

  m_vessel->addVesselEmitter (VesselEmitter (ParticleSystemHelper::createThrustSystem (),
                                             vec3 (0.f, -60.f, 0.f)));
}


void EnemyArrow::slowFire ()
{
  m_lazer->setFireRate (60);
}

void EnemyArrow::fastFire ()
{
  m_lazer->setFireRate (10);
}


void EnemyArrow::update (const float dt)
{
  m_vessel->update (dt, m_positionAndAngle);

  m_lazer->update (dt);
}

void EnemyArrow::drawSolid ()
{
  m_vessel->drawSolid ();

  m_lazer->drawSolid ();
}

void EnemyArrow::drawTransparent ()
{
  m_lazer->drawTransparent ();
}



//----------------------------------------------------------------------------------------------------------------------