#pragma once

#include "cinder/Vector.h"
#include "Collider.h"
#include "Conversions.h"

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
  float     moveCapForce;
  float     moveDistConst;
  float     leanConst;
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

  shared_ptr<Model> getModel () {return m_model;}

  Vec2f getPosition () {return Conversions::fromPhysics (m_body->GetPosition ());}

  // Collider methods
  virtual void collide (float damage, const Vec2f& contactPoint)  {m_life -= damage;}

private:
  bool                    m_isDead;

  const float             m_moveCapForce;
  const float             m_moveDistConst;
  const float             m_leanConst;

  b2Body*                 m_body;

  shared_ptr<Model>       m_model;

  float                   m_life;

#if _DEBUG
  Vec2f                   m_previousDesiredPosition;
#endif
};

//----------------------------------------------------------------------------------------------------------------------

