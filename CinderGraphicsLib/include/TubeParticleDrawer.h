#pragma once

#include "ParticleDrawerInterface.h"

#include "cinder/gl/GlslProg.h"
#include "PhongMaterial.h"

#include <vector>

using namespace std;

class TubeParticle : public Particle
{
  friend class TubeParticleDrawer;

public:
  TubeParticle (uint32_t maxNofPoints, uint32_t segmentLength);

  virtual ~TubeParticle ();

  void define (const Vec3f& position, float size, Vec3f velocity);

  void update ();

private:
  uint32_t        m_nofPoints;
  uint32_t        m_maxNofPoints;
  uint32_t        m_segmentLength;
  uint32_t        m_updateCount;

  vector<Vec3f>   m_points;
  vector<Vec3f>   m_normals;
  vector<float>   m_radie;
};

class TubeParticleDrawer : public ParticleDrawerInterface
{
public:
  TubeParticleDrawer (uint32_t tubeMaxNofPoints, uint32_t tubeSegmentLength);
  virtual ~TubeParticleDrawer ();


  // ParticleDrawerInterface operations
  void createParticles (vector<Particle*> &particles);

  void beforeDraw           ();
  void drawParticle         (const Particle &p, const Vec2f &textureSize);
  void afterDraw            ();

private:
  uint32_t        m_tubeMaxNofPoints;
  uint32_t        m_tubeSegmentLength;

  shared_ptr<PhongMaterial> m_material;
};