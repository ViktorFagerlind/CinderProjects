#pragma once

#include "cinder/Vector.h"

using namespace ci;

class Collider
{
public:
  virtual void decreaseLife (const float lifeToDecrease) = 0;

  virtual float getDamageOutput () const                 = 0;

  virtual bool performSolve () const {return true;}

  virtual void collide (const Collider& c, const Vec2f& contactPoint)
  {
    decreaseLife (c.getDamageOutput ());
  };
};