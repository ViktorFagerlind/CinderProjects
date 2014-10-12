#pragma once

#include "Particle.h"

#include <vector>

using namespace ci;
using namespace std;

class ParticleDrawerInterface
{
public:
  virtual void createParticles (vector<Particle*> &particles)
  { 
    for (uint32_t i = 0; i < particles.size (); i++)
      particles[i] = new Particle();
  };

  virtual void beforeDraw () = 0;

  virtual void drawParticle (const Particle &p, const Vec2f &textureSize) = 0;

  virtual void afterDraw () = 0;
};