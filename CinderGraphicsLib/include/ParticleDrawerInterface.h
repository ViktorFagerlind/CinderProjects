#pragma once

#include "Particle.h"

#include <vector>

using namespace ci;
using namespace std;

class ParticleDrawerInterface
{
public:

  virtual void setup (const vector<Particle> &particles) = 0;
  
  virtual void beforeDraw () = 0;

  virtual void drawParticles (const vector<Particle>& particles, const size_t nofParticles, const vec2 &textureSize) = 0;

  virtual void afterDraw () = 0;
};