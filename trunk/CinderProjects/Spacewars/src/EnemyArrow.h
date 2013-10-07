#pragma once

#include "cinder/Vector.h"
#include "cinder/TriMesh.h"
#include "cinder/Timeline.h"
#include "cinder/gl/Material.h"

#include "World.h"
#include "Vessel.h"
#include "Collider.h"
#include "PositionAndAngle.h"
#include "Weapon.h"


class AreaEmitter;
class PointEmitter;
class Model;
class b2Body;

using namespace ci;
using namespace std;


//----------------------------------------------------------------------------------------------------------------------

class EnemyArrowVessel : public Vessel
{
public:
  EnemyArrowVessel (const VesselDef& vesselDef)
  : Vessel (vesselDef)
  {}

  virtual ~EnemyArrowVessel() {}

  virtual float getDamageOutput () const {return 10.f;}

  // operation from Vessel
  virtual void startedDying ();
  virtual void died ();
};

//----------------------------------------------------------------------------------------------------------------------

class EnemyArrow : public WorldObject
{
public:
  EnemyArrow ();

  void update (const float dt);

  void drawSolid ();

  void drawTransparent ();

  bool isDead () {return m_vessel->isDead ();}

private:
	Anim<PositionAndAngle>	      m_positionAndAngle;

  shared_ptr<EnemyArrowVessel>  m_vessel;

  shared_ptr<Lazer>             m_lazer;
};

//----------------------------------------------------------------------------------------------------------------------

