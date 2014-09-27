#pragma once

#include "cinder/Vector.h"
#include "cinder/TriMesh.h"
#include "cinder/Timeline.h"
#include "cinder/gl/Material.h"

#include "World.h"
#include "Vessel.h"
#include "Collider.h"
#include "PositionAndAngle.h"


class AreaEmitter;
class PointEmitter;
class Model;
class b2Body;

using namespace ci;
using namespace std;


//----------------------------------------------------------------------------------------------------------------------

class EnemyBotVessel : public Vessel
{
public:
  EnemyBotVessel (const VesselDef& vesselDef)
  : Vessel (vesselDef)
  {}

  virtual float getDamageOutput () const {return 10.f;}

  // operation from Vessel
  virtual void startedDying ();
  virtual void died ();
};

//----------------------------------------------------------------------------------------------------------------------

class EnemyBot : public WorldObject
{
public:
  EnemyBot ();

  virtual ~EnemyBot ();

  void update (const float dt);

  void drawSolid ();

  bool isDead () {return m_vessel->isDead ();}

private:
	Anim<PositionAndAngle>	    m_positionAndAngle;

  shared_ptr<EnemyBotVessel>  m_vessel;
};

//----------------------------------------------------------------------------------------------------------------------

