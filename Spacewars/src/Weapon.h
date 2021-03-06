#pragma once

#include "cinder/Vector.h"
#include "cinder/gl/Texture.h"

#include <Box2D/Box2d.h>

#include "Collider.h"
#include "ContactListener.h"
#include "AudioLibrary.h"

using namespace ci;
using namespace std;

class Emitter;
class Vessel;

//----------------------------------------------------------------------------------------------------------------------

class Shot : Collider
{
public:
  Shot ();

  virtual ~Shot ();

  void define (const vec2& position, const vec2& speed, const float rotation, const EntityCategory category);

  void update (const float dt);

  bool isDead () const {return m_isDead;}

  void kill ();

  // Collider methods
  virtual bool performSolve () const {return false;}
  virtual float getDamageOutput () const {return 20.f;}
  virtual void collide (float damage, const vec2& contactPoint);

public:
  bool    m_isDead;

  float   m_rotation;

  b2Body* m_body;
};

//----------------------------------------------------------------------------------------------------------------------

class Weapon
{
public:
  Weapon (const vec3&              relativePos, 
          const shared_ptr<Vessel>  vessel, 
          const EntityCategory      shotCategory, 
          const uint32_t            maxNofShots, 
          const AudioClip           audioClip);

  virtual ~Weapon ();

  void update (const float dt);

  void fire ();

  void setFireRate (const uint32_t fireRate) {m_fireRate = fireRate;}

  virtual void drawSolid () {};

  virtual void drawTransparent () {};

protected:
  vec3               m_relativePos;

  const uint32_t      m_maxNofShots;
  uint32_t            m_nofShots;

  uint32_t            m_fireCounter;
  uint32_t            m_fireRate;

  vector<shared_ptr<Shot>> m_shots;

  shared_ptr<Emitter> m_emitter;
  const uint32_t      m_emitterTime;

  shared_ptr<Vessel>  m_vessel;

  EntityCategory      m_shotCategory;

  const AudioClip     m_audioClip;
};

//----------------------------------------------------------------------------------------------------------------------

class Lazer : public Weapon
{
public:
  Lazer (const vec3& relativePos, ColorAf color, const shared_ptr<Vessel> vessel, EntityCategory shotCategory, const uint32_t maxNofShots, const bool noSound=false);

  virtual ~Lazer ();

  void drawTransparent ();

private:
  void fire (const Vessel *vessel);

private:
  gl::Texture m_shotTexture;
  ColorAf     m_color;
};

//----------------------------------------------------------------------------------------------------------------------

