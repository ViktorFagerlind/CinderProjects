#pragma once

#include "cinder/Vector.h"
#include "cinder/gl/Texture.h"

#include <Box2D/Box2d.h>

#include "Collider.h"

using namespace ci;
using namespace std;

class Emitter;
class Vessel;

//----------------------------------------------------------------------------------------------------------------------

class Shot : Collider
{
public:
  Shot ();

  void define (const Vec2f& position, const Vec2f& speed, float rotation);

  void update (const float dt);

  bool isDead () const {return m_isDead;}

  void kill ();

  // Collider methods
  virtual bool performSolve () const {return false;}
  virtual float getDamageOutput () const {return 20.f;}
  virtual void collide (float damage, const Vec2f& contactPoint);

public:
  bool    m_isDead;

  float   m_rotation;

  b2Body* m_body;
};

//----------------------------------------------------------------------------------------------------------------------

class Weapon
{
public:
  Weapon (const Vec3f& relativePos);

  virtual ~Weapon ();

  void update (const float dt, const Vessel *vessel);

  void fire (const Vessel *vessel);

  virtual void drawSolid () {};

  virtual void drawTransparent () {};

protected:
  Vec3f             m_relativePos;

  const uint32_t    m_maxNofShots;
  uint32_t          m_nofShots;

  uint32_t          m_fireCounter;
  const uint32_t    m_fireRate;

  vector<shared_ptr<Shot>> m_shots;

  Emitter          *m_emitter;
  const uint32_t    m_emitterTime;
};

//----------------------------------------------------------------------------------------------------------------------

class Lazer : public Weapon
{
public:
  Lazer (const Vec3f& relativePos, ColorAf color);

  virtual ~Lazer ();

  void drawTransparent ();

private:
  void fire (const Vessel *vessel);

private:
  gl::Texture m_shotTexture;
  ColorAf     m_color;
};

//----------------------------------------------------------------------------------------------------------------------

