#pragma once
#include "cinder/Vector.h"
#include "cinder/Matrix.h"

#include "PhysicsObject.h"

using namespace ci;
using namespace std;

class DynamicObject : public PhysicsObject
{
  public:
  DynamicObject(float mass, Matrix44f inertia, Vec3f& cog);
	void update();
	void draw();
  void applyForce(Vec3f force);
  void resetForce();
  void applyTorque(Vec3f pointOfAttack, Vec3f force); //point of attack and force in global reference frame
  void resetTorque();

private:
  Matrix44f mInertia;
  Vec3f mForceSum;
  Vec3f mTorqueSum;
};
