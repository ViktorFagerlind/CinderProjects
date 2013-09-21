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

class Enemy1 : public WorldObject
{
public:
  Enemy1 ();

  virtual ~Enemy1 ();

  void update (const float dt);

  void draw ();

  bool isDead () {return m_vessel->isDead ();}

private:
	Anim<PositionAndAngle>	m_positionAndAngle;

  shared_ptr<Vessel>      m_vessel;
};

//----------------------------------------------------------------------------------------------------------------------

