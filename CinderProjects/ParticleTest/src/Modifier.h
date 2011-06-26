#pragma once

#include "Particle.h"

class Modifier 
{
public:
  Modifier() {};

  virtual ~Modifier() {};

  virtual void apply (Particle *const particle) = 0;

  virtual void update () {};
};