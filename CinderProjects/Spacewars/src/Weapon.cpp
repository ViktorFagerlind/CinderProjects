#include "Weapon.h"

#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/app/App.h"
#include "cinder/ObjLoader.h"

#include "ParticleSystemHelper.h"
#include "Emitter.h"
#include "ImageLibrary.h"

#include "Macros.h"
#include "World.h"
#include "Conversions.h"

using namespace ci;
using namespace ci::app;

//----------------------------------------------------------------------------------------------------------------------

Shot::Shot ()
: m_speed    (Vec2f (0.f, 1000.f)),
  m_isDead   (true)
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
  fixtureDef.filter.maskBits     = EntityCategory_Enemies_E;

	m_body->CreateFixture (&fixtureDef);

  m_body->SetUserData (this);
}

void Shot::define (const Vec2f& position)
{
  m_isDead   = false;

  // Turn on collision detection
  b2Filter filter;
  filter.categoryBits = EntityCategory_OurShots_E; // deactivated from the start
  filter.maskBits     = EntityCategory_Enemies_E;

  m_body->GetFixtureList ()->SetFilterData (filter);

  m_body->SetTransform (Conversions::toPhysics (position), 0.f);
  m_body->SetLinearVelocity (Conversions::toPhysics (m_speed));
}

void Shot::kill ()
{
  m_isDead = true;

  // Turn off collision detection
  b2Filter filter;
  filter.categoryBits = 0;
  filter.maskBits     = EntityCategory_Enemies_E;
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

  if (position.x >  1200.f ||
      position.x < -1200.f || 
      position.y >  1000.f ||
      position.y < -1000.f)
    kill ();
}


void Shot::draw ()
{
  const float w=5.f, h=60.f;

  float textureCoordinates[] = {0.f, 0.f,
                                1.f, 0.f,
                                1.f, 1.f,
                                0.f, 0.f,
                                1.f, 1.f,
                                0.f, 1.f};

  Vec2f position = Conversions::fromPhysics (m_body->GetPosition ());

  Vec3f vertices[] = {Conversions::Vec2fTo3f (position) + Vec3f (-w, h, 0.f),
                      Conversions::Vec2fTo3f (position) + Vec3f ( w, h, 0.f),
                      Conversions::Vec2fTo3f (position) + Vec3f ( w,-h, 0.f),
                      Conversions::Vec2fTo3f (position) + Vec3f (-w, h, 0.f),
                      Conversions::Vec2fTo3f (position) + Vec3f ( w,-h, 0.f),
                      Conversions::Vec2fTo3f (position) + Vec3f (-w,-h, 0.f)};

  gl::color (1.f, 0.8f, 1.f, 1.f);

  gl::disable (GL_LIGHTING);
  gl::enableAlphaBlending ();
	gl::enableAdditiveBlending ();

  // Set array pointers
	glVertexPointer   (3, GL_FLOAT, 0, vertices);
	glTexCoordPointer (2, GL_FLOAT, 0, textureCoordinates);

  // Enable arrays
	glEnableClientState (GL_VERTEX_ARRAY);
	glEnableClientState (GL_TEXTURE_COORD_ARRAY);

  // Draw arrays
	glDrawArrays (GL_TRIANGLES, 0, 6);
  
  // Disable arrays
	glDisableClientState (GL_VERTEX_ARRAY);
	glDisableClientState (GL_TEXTURE_COORD_ARRAY);

  gl::disableAlphaBlending ();
  gl::enable (GL_LIGHTING);
}

//----------------------------------------------------------------------------------------------------------------------

Weapon::Weapon (const Vec2f& relativePos)
: m_relativePos (relativePos),
  m_fireCounter (0),
  m_fireRate    (15),
  m_emitterTime (5),
  m_maxNofShots (100),
  m_nofShots    (0)
{
  for (uint32_t i=0; i<m_maxNofShots; i++)
  {
    shared_ptr<Shot> shot (new Shot ());
    m_shots.push_back (shot);
  }

  m_shotTexture = ImageLibrary::getSingleton ().getTexture ("particle1.jpg");


  m_emitter = ParticleSystemHelper::createSparks ();
}

Weapon::~Weapon ()
{
}

void Weapon::update (const float dt, const Vec2f& parentPos)
{
  m_position = parentPos + m_relativePos;

  if (m_fireCounter == m_fireRate) // Fire if it is time
  {
    fire (parentPos);
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
  m_emitter->setPosition (Conversions::Vec2fTo3f (m_position - Vec2f (0.f, 25.f)));
}

void Weapon::draw ()
{
  // Draw shots
  m_shotTexture.bind ();
  for (uint32_t i = 0; i < m_nofShots; i++)
  {
    m_shots[i]->draw ();
  }
  m_shotTexture.unbind ();
}

void Weapon::fire (const Vec2f& parentPos)
{
  if (m_nofShots >= m_maxNofShots)
    return;

  // Create new shot
  m_shots[m_nofShots]->define (m_position);

  // Emit sparks
  m_emitter->unpause ();

  m_nofShots++;
}
