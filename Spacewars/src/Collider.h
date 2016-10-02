#pragma once

#include "cinder/Vector.h"

using namespace ci;

class Collider
{
public:
  virtual bool performSolve () const {return true;}

  virtual float getDamageOutput () const                          = 0;

  virtual void collide (float damage, const vec2& contactPoint)  = 0;
};