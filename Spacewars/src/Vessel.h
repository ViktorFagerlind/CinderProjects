#pragma once

#include "cinder/Vector.h"
#include "Collider.h"
#include "Conversions.h"
#include "ContactListener.h"

#include <vector>

class b2Shape;
class Model;
class Emitter;
class b2Body;
class PositionAndAngle;
class Vessel;

using namespace ci;
using namespace std;


//----------------------------------------------------------------------------------------------------------------------

class VesselEmitter
{
public:
  VesselEmitter (shared_ptr<Emitter> emitter, const vec3& relativePos);

public:
  vec3               m_relativePos;
  shared_ptr<Emitter> m_emitter;
};

//----------------------------------------------------------------------------------------------------------------------

class VesselDef
{
public:
  VesselDef ()
  : position            (vec2::zero ()),
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
  vec2           position;
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

  void eliminate ();
  bool isDead () const {return m_state == State_Dead_E;}

  shared_ptr<Model> getModel () {return m_model;}

  vec3 getPosition () const {return Conversions::fromPhysics3f (m_body->GetPosition ());}

  const vec3& getRotation () const {return m_rotation;}
  void setRotation (const vec3& rotation) {m_rotation = rotation;}
  vec3 vesselPositionToWorld (const vec3& vec) const;
  vec3 vesselRotationToWorld (const vec3& vec) const;

  void  addVesselEmitter (const VesselEmitter& vesselEmitter);
  void  clearVesselEmitters ();

  // Collider methods
  virtual void collide (float damage, const vec2& contactPoint)  {m_life -= damage;}

  // Methods for dying, like explosions etc
  virtual void startedDying () {};
  virtual void died () {};

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

  vec3                   m_rotation;

  shared_ptr<Model>       m_model;

  float                   m_life;

  Vessel::State           m_state;

  vector<VesselEmitter>   m_vesselEmitters;


#if _DEBUG
  vec2                   m_previousDesiredPosition;
#endif
};

//----------------------------------------------------------------------------------------------------------------------

