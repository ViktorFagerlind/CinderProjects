#pragma once

class Collider
{
public:
  virtual void decreaseLife (const float lifeToDecrease) = 0;

  virtual float getDamageOutput () const                 = 0;

  virtual void collide (const Collider& c)
  {
    decreaseLife (c.getDamageOutput ());
  };
};