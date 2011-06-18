#pragma once

#include "Particle.h"

class Modifier 
{
public:
  virtual void apply (Particle *const particle) = 0;
};