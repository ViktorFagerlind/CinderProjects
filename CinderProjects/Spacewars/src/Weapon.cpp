#include "Weapon.h"

#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/app/App.h"
#include "cinder/ObjLoader.h"

#include "ParticleSystemHelper.h"
#include "Emitter.h"
#include "ImageLibrary.h"
#include "Vessel.h"

#include "Macros.h"
#include "World.h"
#include "Conversions.h"

using namespace ci;
using namespace ci::app;

//----------------------------------------------------------------------------------------------------------------------

Shot::Shot ()
: m_isDead   (true)
{
  // -------------- setup physics ----------------
	// create a dynamic body
	b2BodyDef bodyDef;
  bodyDef.type = b2_kinematicBody;
	bodyDef.position.Set (Conversions::toPhysics (0.f), Conversions::toPhysics (0.f));

	// Set userData to point at the class if needed to be used later on...
	bodyDef.userData = this;
	m_body = World::getPhysicsWorld ().CreateBody (&bodyDef);

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox (Conversions::toPhysics (5.f), Conversions::toPhysics (60.f));

	b2FixtureDef fixtureDef;
	fixtureDef.shape               = &dynamicBox;
	//fixtureDef.isSensor            = true;
  fixtureDef.filter.categoryBits = 0; // deactivated from the start
  fixtureDef.filter.maskBits     = EntityCategory_Enemies_E | EntityCategory_EnemySwarm_E;

	m_body->CreateFixture (&fixtureDef);

  m_body->SetUserData (this);
}

void Shot::define (const Vec2f& position, const Vec2f& speed, float rotation)
{
  m_isDead   = false;

  m_rotation = rotation;

  // Turn on collision detection
  b2Filter filter;
  filter.categoryBits = EntityCategory_OurShots_E; // deactivated from the start
  filter.maskBits     = EntityCategory_Enemies_E | EntityCategory_EnemySwarm_E;

  m_body->GetFixtureList ()->SetFilterData (filter);

  m_body->SetTransform (Conversions::toPhysics (position), 0.f);
  m_body->SetLinearVelocity (Conversions::toPhysics (speed));
}

void Shot::kill ()
{
  m_isDead = true;

  // Turn off collision detection
  b2Filter filter;
  filter.categoryBits = 0;
  filter.maskBits     = EntityCategory_Enemies_E | EntityCategory_EnemySwarm_E;
  m_body->GetFixtureList ()->SetFilterData (filter);
}

void Shot::collide (float damage, const Vec2f& contactPoint)
{
  // Create small explosion at contact
  ParticleSystemHelper::createMiniExplosion ()->setPosition (Conversions::Vec2fTo3f (contactPoint));
  
  kill ();
};


void Shot::update (const float dt)
{
  Vec2f position = Conversions::fromPhysics (m_body->GetPosition ());

  if (position.x >   20.f + World::getSingleton ().getDownRight ().x ||
      position.x <  -20.f + World::getSingleton ().getTopLeft   ().x || 
      position.y >  100.f + World::getSingleton ().getTopLeft   ().y ||
      position.y < -100.f + World::getSingleton ().getDownRight ().y)
    kill ();
}

//----------------------------------------------------------------------------------------------------------------------

Weapon::Weapon (const Vec3f& relativePos)
: m_relativePos (relativePos),
  m_fireCounter (0),
  m_fireRate    (10),
  m_emitterTime (5),
  m_maxNofShots (100),
  m_nofShots    (0)
{
  for (uint32_t i=0; i<m_maxNofShots; i++)
  {
    shared_ptr<Shot> shot (new Shot ());
    m_shots.push_back (shot);
  }

  m_emitter = ParticleSystemHelper::createSparks ();
}

Weapon::~Weapon ()
{
}

void Weapon::update (const float dt, const Vessel *vessel)
{
  if (m_fireCounter == m_fireRate) // Fire if it is time
  {
    fire (vessel);
    m_fireCounter = 0;
  }
  if (m_fireCounter == m_emitterTime) // Stops the fire sparks
    m_emitter->pause (); 
  m_fireCounter++;

  // Update shots
  for (uint32_t i = 0; i < m_nofShots; i++)
  {
    Shot *s = m_shots[i].get ();

    while (s->isDead ())
    {
      // Switch dead shot
      shared_ptr<Shot> tmpShot  = m_shots[m_nofShots-1];
      m_shots[m_nofShots-1]     = m_shots[i];
      m_shots[i]                = tmpShot;

      s = tmpShot.get ();
      m_nofShots--;

      if (m_nofShots == 0)
        return;    
    }

    s->update (dt);
  }

  // Move emitter
  m_emitter->setPosition (vessel->vesselPositionToWorld (m_relativePos));
}

void Weapon::fire (const Vessel *vessel)
{
  if (m_nofShots >= m_maxNofShots)
    return;

  Vec2f worldPosition = Conversions::Vec3fTo2f (vessel->vesselPositionToWorld (m_relativePos + Vec3f (0.f, 20.f, 0.f)));
  Vec2f worldSpeed    = Conversions::Vec3fTo2f (vessel->vesselRotationToWorld (Vec3f (0.f, 2000.f, 0.f)));

  // Create new shot
  m_shots[m_nofShots]->define (worldPosition, worldSpeed, vessel->getRotation ().z);

  // Emit sparks
  m_emitter->unpause ();

  m_nofShots++;
}

//----------------------------------------------------------------------------------------------------------------------

Lazer::Lazer (const Vec3f& relativePos, ColorAf color)
: Weapon (relativePos),
  m_color (color)
{
  m_shotTexture = ImageLibrary::getSingleton ().getTexture ("basic particle 1.png");
}


Lazer::~Lazer ()
{
}

void Lazer::drawTransparent ()
{
  const float w=3.f, h=60.f;

  // Enable arrays
	glEnableClientState (GL_VERTEX_ARRAY);
	glEnableClientState (GL_TEXTURE_COORD_ARRAY);
	glEnableClientState (GL_COLOR_ARRAY);

  m_shotTexture.bind ();

  for (uint32_t i = 0; i < m_nofShots; i++)
  {
    Shot *s = m_shots[i].get ();

    Emitter::drawBillboard (Conversions::fromPhysics3f (s->m_body->GetPosition ()),  Vec2f (w, h), m_color, s->m_rotation);
  }

  m_shotTexture.unbind ();

  // Disable arrays
	glDisableClientState (GL_VERTEX_ARRAY);
	glDisableClientState (GL_TEXTURE_COORD_ARRAY);
	glDisableClientState (GL_COLOR_ARRAY);

}