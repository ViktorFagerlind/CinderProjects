#pragma once

#include "ParticleDrawerInterface.h"
#include "cinder/gl/gl.h"

#include "SpriteData.h"
#include "SpriteDataParser.h"
#include <vector>

using namespace std;

class AnimParticleDrawer : public ParticleDrawerInterface
{
public:
  AnimParticleDrawer (shared_ptr<vector<SpriteData>> spriteData);
  virtual ~AnimParticleDrawer ();

  // ParticleDrawerInterface operations
  void beforeDraw ();
  void drawParticle (const Particle &p, const Vec2f &textureSize);
  void afterDraw ();

private:
  shared_ptr<vector<SpriteData>> m_spriteData;
};