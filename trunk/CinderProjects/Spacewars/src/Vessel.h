#pragma once

#include "cinder/Vector.h"
#include "Collider.h"

class b2Shape;
class Model;
class b2Body;
class PositionAndAngle;

using namespace ci;
using namespace std;

//----------------------------------------------------------------------------------------------------------------------

class VesselDef
{
public:
  VesselDef ()
  : position          (Vec2f::zero ()),
    angle               (toRadians (180.f)),
    isEnemy             (true),
    initialLife         (100.f),
    modelName           ("error.obj"),
    bodyLinearDamping   (10.f),
    bodyAngularDamping  (15.f),
    fixtureShape        (NULL),
    fixtureDensity      (1.f)
  {
  }

public:
  Vec2f     position;
  float     angle;
  bool      isEnemy;
  float     initialLife;
  string    modelName;

  float     bodyLinearDamping;
  float     bodyAngularDamping;
  b2Shape*  fixtureShape;
  float     fixtureDensity;
};

//----------------------------------------------------------------------------------------------------------------------

class Vessel : Collider
{
public:
  Vessel (const VesselDef& vesselDef);

  virtual ~Vessel ();

  void update (const float dt, const PositionAndAngle& positionAndAngle);

  void draw ();

  void eliminate () {m_isDead = true;}

  bool isDead () const {return m_isDead;}

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

#if _DEBUG
  const PositionAndAngle *m_previousPositionAndAngle;
#endif
};

//----------------------------------------------------------------------------------------------------------------------

