#pragma once

#include "ParticleDrawerInterface.h"

#include "cinder/gl/gl.h"

class BillboardParticleDrawer : public ParticleDrawerInterface
{
public:
  BillboardParticleDrawer ();
  virtual ~BillboardParticleDrawer ();

  // ParticleDrawerInterface operations
  void beforeDraw ();
  void drawParticle (const Particle &p, const vec2 &textureSize);
  void afterDraw ();
};