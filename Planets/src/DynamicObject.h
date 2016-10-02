#pragma once

#include "cinder/Vector.h"
#include "PhysicsObject.h"


using namespace ci;
using namespace std;

class DynamicObject:public PhysicsObject
{
public:
  DynamicObject (const float mass, const vec3& position, float radius);
  virtual ~DynamicObject ();
  
  void setVelocity(const vec3& velocity){mVelocity = velocity;}

  void setRotationVelocity(const vec3& rotationVelocity){mRotationVelocity = rotationVelocity;}

  void applyForce(const vec3& force){mForceSum = mForceSum + force;}

  void move();

protected:
  vec3 mVelocity;
  vec3 mAcceleration;
  vec3 mForceSum;

  vec3 mRotationVelocity;
  vec3 mRotation;
};


