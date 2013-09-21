#pragma once

#include "cinder/Vector.h"
#include "cinder/TriMesh.h"
#include "cinder/Timeline.h"
#include "cinder/gl/Material.h"

#include "Collider.h"


class AreaEmitter;
class PointEmitter;
class Model;
class b2Body;

using namespace ci;
using namespace std;

//----------------------------------------------------------------------------------------------------------------------

class PositionAndAngle
{
public:
  PositionAndAngle ()
  : m_position (0.f, 0.f), 
    m_angle (0.f)
  {
  }

  PositionAndAngle (float x, float y, float angle)
  : m_position (x,y), 
    m_angle (angle)
  {
  }

  PositionAndAngle operator * (const float rhs) const 
  {
    return PositionAndAngle (m_position.x * rhs, 
                             m_position.y * rhs, 
                             m_angle      * rhs);
  }

  PositionAndAngle operator + (const PositionAndAngle& rhs) const 
  {
    return PositionAndAngle (m_position.x + rhs.m_position.x, 
                             m_position.y + rhs.m_position.y, 
                             m_angle      + rhs.m_angle);
  }

public:
  float m_angle;
  Vec2f m_position;
};

//----------------------------------------------------------------------------------------------------------------------

class EnemyVessel : Collider
{
public:
  EnemyVessel ();

  virtual ~EnemyVessel ();

  void update (float dt);

  void draw ();

  void eliminate () {m_isDead = true;};

  bool isDead () const {return m_isDead;};

  // Collider methods
  virtual void decreaseLife (const float lifeToDecrease) {m_life -= lifeToDecrease;}

  virtual float getDamageOutput () const {return 1.f;}

  virtual void collide (const Collider& c)
  {
    Collider::collide (c);
  };

private:
  bool                    m_isDead;

  b2Body*                 m_body;

  shared_ptr<Model>       m_model;

  float                   m_life;

	Anim<PositionAndAngle>	m_positionAndAngle;
};

//----------------------------------------------------------------------------------------------------------------------

