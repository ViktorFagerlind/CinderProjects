#pragma once

#include "cinder/Vector.h"
#include "cinder/TriMesh.h"
#include "cinder/gl/Material.h"

#include "Weapon.h"
#include "Collider.h"

class AreaEmitter;
class PointEmitter;
class b2Body;
class Model;

using namespace ci;
using namespace std;

//----------------------------------------------------------------------------------------------------------------------

class Engine
{
public:
  Engine ();

  virtual ~Engine ();

  void update (float dt, const Vec2f& pos);

  void draw ();

private:
  AreaEmitter  *m_baseEmitter;
  PointEmitter *m_flareEmitter;
};

//----------------------------------------------------------------------------------------------------------------------

class MainVessel : Collider
{
public:
  MainVessel ();

  virtual ~MainVessel ();

  void update (float dt, const Vec2f& touchPos);

  void draw ();

  // Collider methods
  virtual void decreaseLife (const float lifeToDecrease) {m_life -= lifeToDecrease;}

  virtual float getDamageOutput () const {return 1.f;}

  virtual void collide (const Collider& c)
  {
    Collider::collide (c);
  };

private:
  Engine             m_engine;

  b2Body*            m_body;

  shared_ptr<Model>  m_model;

  shared_ptr<Weapon> m_leftLaser;
  shared_ptr<Weapon> m_rightLaser;
  
  float         m_life;
};

//----------------------------------------------------------------------------------------------------------------------

