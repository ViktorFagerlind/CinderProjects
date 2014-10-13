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
  TubeParticleDrawer (uint32_t tubeMaxNofPoints, 
                      uint32_t tubeSegmentLength, 
                      ColorAf  ambient            = ColorAf (0.25f, 0.25f, 0.25f, 1.f),
                      ColorAf  diffuse            = ColorAf (0.35f, 0.55f, 0.70f, .5f),
                      ColorAf  specular           = ColorAf (0.70f, 0.90f, 0.90f, 1.f),
                      float    shininess          = 10.f);

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