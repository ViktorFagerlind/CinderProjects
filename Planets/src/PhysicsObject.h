#pragma once

#include "cinder/Vector.h"

using namespace ci;
using namespace std;

class PhysicsObject
{
public:
  PhysicsObject (const float mass, const vec3& position, float radius);

  const vec3& getPosition () const {return mPosition;}
  float getMass () const {return mMass;}
  float getRadius () const {return mRadius;}

protected:
  vec3 mPosition;

  float mRadius;

  float mMass;
};


