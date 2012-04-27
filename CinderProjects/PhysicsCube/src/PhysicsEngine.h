#pragma once
#include "cinder/Vector.h"
#include "cinder/Matrix.h"
#include "PhysicsObject.h"
#include "DynamicObject.h"
#include "StaticObject.h"
#include "cinder/Timer.h"

using namespace ci;
using namespace std;

class PhysicsEngine
{
public:
  PhysicsEngine();
  State getState(PhysicsObject physicsObject);
  void  setState(PhysicsObject physicsObject);

	void update ();
	void draw();
  void applyForce(PhysicsObject physicsObject, Vec3f force);
  void applyTorque(PhysicsObject physicsObject, Vec3f pointOfAttack, Vec3f force);

  //Collision detection methods
  Vec3f getCollisionPoint();

  DynamicObject *cube;
  StaticObject *plane;
  DynamicObject *sphere;
  Timer mTimer;
  float mLastTimeStamp;
};
