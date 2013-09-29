#pragma once

#include "cinder/Vector.h"
#include "Collider.h"
#include "Conversions.h"
#include "ContactListener.h"

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
  : position            (Vec2f::zero ()),
    angle               (toRadians (180.f)),
    category            (EntityCategory_Enemies_E),
    initialLife         (100.f),
    timeToDie           (0.2f),
    modelName           ("error.obj"),
    bodyLinearDamping   (10.f),
    bodyAngularDamping  (15.f),
    fixtureDensity      (1.f),
    fixedRotation       (false)
  {
  }

public:
  Vec2f           position;
  float           angle;
  EntityCategory  category;
  float           initialLife;
  float           timeToDie;
  float           moveCapForce;
  float           moveDistConst;
  float           leanConst;
  string          modelName;
  bool            fixedRotation;

  float           bodyLinearDamping;
  float           bodyAngularDamping;
  float           fixtureDensity;
};


//----------------------------------------------------------------------------------------------------------------------

class Vessel : Collider
{
public:
  Vessel (const VesselDef& vesselDef);

  virtual ~Vessel ();

  void update (const float dt, const PositionAndAngle& positionAndAngle);

  void drawSolid ();

  void eliminate () {m_state = State_Dead_E;}

  virtual void startedDying () {};

  virtual void died () {};

  bool isDead () const {return m_state == State_Dead_E;}

  shared_ptr<Model> getModel () {return m_model;}

  Vec3f getPosition () const {return Conversions::fromPhysics3f (m_body->GetPosition ());}

  const Vec3f& getRotation () const {return m_rotation;}
  void setRotation (const Vec3f& rotation) {m_rotation = rotation;}

  Vec3f vesselPositionToWorld (const Vec3f& vec) const;

  Vec3f vesselRotationToWorld (const Vec3f& vec) const;


  // Collider methods
  virtual void collide (float damage, const Vec2f& contactPoint)  {m_life -= damage;}

protected:
  enum State
  {
    State_Living_E,
    State_Dying_E,
    State_Dead_E
  };

protected:
  const float             m_moveCapForce;
  const float             m_moveDistConst;
  const float             m_leanConst;

  const float             m_timeToDie;
  float                   m_timeOfDeath;

  b2Body*                 m_body;

  Vec3f                   m_rotation;

  shared_ptr<Model>       m_model;

  float                   m_life;

  Vessel::State           m_state;

#if _DEBUG
  Vec2f                   m_previousDesiredPosition;
#endif
};

//----------------------------------------------------------------------------------------------------------------------

