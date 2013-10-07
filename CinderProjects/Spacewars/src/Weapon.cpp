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
  fixtureDef.filter.categoryBits = 0; // deactivated from the start
  fixtureDef.filter.maskBits     = 0; // -"-

	m_body->CreateFixture (&fixtureDef);

  m_body->SetUserData (this);
}

Shot::~Shot ()
{
  World::getPhysicsWorld ().DestroyBody (m_body);
}

void Shot::define (const Vec2f& position, const Vec2f& speed, const float rotation, const EntityCategory category)
{
  m_isDead   = false;

  m_rotation = rotation;

  // Turn on collision detection
  b2Filter filter;
  filter.categoryBits = category;
  if (category == EntityCategory_OurShots_E)
    filter.maskBits = EntityCategory_Enemies_E | EntityCategory_EnemySwarm_E;
  else
    filter.maskBits = EntityCategory_OurShip_E;

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
  filter.maskBits     = 0;
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

Weapon::Weapon (const Vec3f& relativePos, const shared_ptr<Vessel> vessel, EntityCategory shotCategory, const uint32_t maxNofShots)
: m_relativePos (relativePos),
  m_fireCounter (0),
  m_fireRate    (10),
  m_emitterTime (5),
  m_maxNofShots (maxNofShots),
  m_nofShots    (0),
  m_vessel      (vessel),
  m_shotCategory (shotCategory)
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
  m_emitter->kill ();
}

void Weapon::update (const float dt)
{
  if (m_fireCounter >= m_fireRate) // Fire if it is time (use >= since the fire rate may change)
  {
    fire ();
    m_fireCounter = 0;
  }
  if (m_fireCounter >= m_emitterTime) // Stops the fire sparks (use >= since the fire rate may change)
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
  m_emitter->setPosition (m_vessel->vesselPositionToWorld (m_relativePos));
}

void Weapon::fire ()
{
  if (m_nofShots >= m_maxNofShots)
    return;

  Vec2f worldPosition = Conversions::Vec3fTo2f (m_vessel->vesselPositionToWorld (m_relativePos + Vec3f (0.f, 20.f, 0.f)));
  Vec2f worldSpeed    = Conversions::Vec3fTo2f (m_vessel->vesselRotationToWorld (Vec3f (0.f, 2000.f, 0.f)));

  // Create new shot
  m_shots[m_nofShots]->define (worldPosition, worldSpeed, m_vessel->getRotation ().z, m_shotCategory);

  // Emit sparks
  m_emitter->unpause ();

  m_nofShots++;
}

//----------------------------------------------------------------------------------------------------------------------

Lazer::Lazer (const Vec3f& relativePos, ColorAf color, const shared_ptr<Vessel> vessel, EntityCategory shotCategory, const uint32_t maxNofShots)
: Weapon (relativePos, vessel, shotCategory, maxNofShots),
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