#pragma once

#include "cinder/Vector.h"

using namespace ci;
using namespace std;

class PhysicsObject
{
public:
  PhysicsObject (const float mass, const Vec3f& position, float radius);

  const Vec3f& getPosition () const {return mPosition;}
  float getMass () const {return mMass;}
  float getRadius () const {return mRadius;}

protected:
  Vec3f mPosition;

  float mRadius;

  float mMass;
};


