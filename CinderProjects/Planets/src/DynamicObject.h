#pragma once

#include "cinder/Vector.h"
#include "PhysicsObject.h"


using namespace ci;
using namespace std;

class DynamicObject:public PhysicsObject
{
public:
  DynamicObject (const float mass, const Vec3f& position, float radius);
  virtual ~DynamicObject ();
  
  void setVelocity(const Vec3f& velocity){mVelocity = velocity;}

  void setRotationVelocity(const Vec3f& rotationVelocity){mRotationVelocity = rotationVelocity;}

  void applyForce(const Vec3f& force){mForceSum = mForceSum + force;}

  void move();

protected:
  Vec3f mVelocity;
  Vec3f mAcceleration;
  Vec3f mForceSum;

  Vec3f mRotationVelocity;
  Vec3f mRotation;
};


