#pragma once

#include "cinder/Vector.h"
#include "cinder/gl/Texture.h"

#include <Box2D/Box2d.h>

#include "Collider.h"

using namespace ci;
using namespace std;

class Emitter;

//----------------------------------------------------------------------------------------------------------------------

class Shot : Collider
{
public:
  Shot ();

  void define (const Vec2f& position);

  void update (const float dt);

  void draw ();

  bool isDead () const {return m_isDead;}

  void kill ();

  // Collider methods
  virtual void decreaseLife (const float lifeToDecrease) {}

  virtual float getDamageOutput () const {return 1.f;}

  virtual bool performSolve () const {return false;}

  virtual void collide (const Collider& c, const Vec2f& contactPoint);

private:
  Vec2f   m_speed;

  bool    m_isDead;

  b2Body* m_body;
};

//----------------------------------------------------------------------------------------------------------------------

class Weapon
{
public:
  Weapon (const Vec2f& relativePos);

  virtual ~Weapon ();

  void update (const float dt, const Vec2f& parentPos);

  void draw ();

private:
  void fire (const Vec2f& parentPos);

private:
  Vec2f             m_relativePos;

  const uint32_t    m_maxNofShots;
  uint32_t          m_nofShots;

  uint32_t          m_fireCounter;
  const uint32_t    m_fireRate;

  vector<shared_ptr<Shot>> m_shots;

  gl::Texture       m_shotTexture;

  Emitter          *m_emitter;
  const uint32_t    m_emitterTime;

  Vec2f             m_position;
};

//----------------------------------------------------------------------------------------------------------------------

