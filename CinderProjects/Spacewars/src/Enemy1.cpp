#include "EnemyVessel.h"

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


EnemyVessel::EnemyVessel ()
: m_isDead (false)
{
  // -------------- setup animation ----------
  m_positionAndAngle = PositionAndAngle (-300, 700, toRadians (180.f));
  timeline().appendTo (&m_positionAndAngle, PositionAndAngle (-200,-400, toRadians (180.f)), 2.f, EaseNone());
	timeline().appendTo (&m_positionAndAngle, PositionAndAngle (   0, 200, toRadians (180.f)), 1.f, EaseNone());
	timeline().appendTo (&m_positionAndAngle, PositionAndAngle (   0,-300, toRadians (250.f)), 2.f, EaseNone());
	timeline().appendTo (&m_positionAndAngle, PositionAndAngle ( 300,   0, toRadians (300.f)), 1.f, EaseNone());
  timeline().appendTo (&m_positionAndAngle, PositionAndAngle ( 300, 800, toRadians (360.f)), 2.f, EaseNone())
    .finishFn (bind (&EnemyVessel::eliminate, this));

  // -------------- setup physics ----------------
	// create a dynamic body
	b2BodyDef bodyDef;
	bodyDef.type            = b2_dynamicBody;
  bodyDef.position        = Conversions::toPhysics (m_positionAndAngle.value ().m_position);
  bodyDef.angle           = m_positionAndAngle.value ().m_angle;
  bodyDef.linearDamping   = 10.f;
  bodyDef.angularDamping  = 15.f;
	bodyDef.userData        = this;
	m_body = World::getPhysicsWorld ().CreateBody (&bodyDef);

  // create collision shape
  b2CircleShape circle;
  circle.m_p.Set(0,0);
  circle.m_radius = Conversions::toPhysics (45.f);

  // create fixture
	b2FixtureDef fixtureDef;
	fixtureDef.shape                = &circle;
	fixtureDef.density              = 1.0f;
	fixtureDef.friction             = 0.3f;
	fixtureDef.restitution          = 0.f; // bounce
  fixtureDef.filter.categoryBits  = Enemies_E;
  fixtureDef.filter.maskBits      = OurShip_E | OurShots_E | Enemies_E;
	m_body->CreateFixture (&fixtureDef);

  // Set life
  m_life = 100.f;

  // Set model
  m_model = ModelLibrary::getSingleton ().getModel ("enemy_vessel");
}

EnemyVessel::~EnemyVessel ()
{
  World::getPhysicsWorld ().DestroyBody (m_body);
}

void EnemyVessel::update (float dt)
{
  //------------ Handle position ---------------------
  const float distCost = 100.f;
  b2Vec2 distance = Conversions::toPhysics (m_positionAndAngle.value ().m_position) - m_body->GetPosition ();
  b2Vec2 force    = distCost * distance;
  m_body->ApplyForceToCenter (force);

  //------------ Handle rotation ---------------------
  const float rotationConst = 1.f;
  m_body->ApplyAngularImpulse (-rotationConst * (m_body->GetAngle ()-m_positionAndAngle.value ().m_angle));
  // console () << "Wanted angle: " << m_angle << " Current angle: " << m_body->GetAngle () << "\n";
}

void EnemyVessel::draw ()
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
  gl::drawSolidCircle (m_positionAndAngle.value ().m_position, 10);
  gl::enable (GL_LIGHTING);
#endif
}

//----------------------------------------------------------------------------------------------------------------------