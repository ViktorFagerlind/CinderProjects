#pragma once

#include "ParticleDrawerInterface.h"

#include "cinder/gl/gl.h"
#include <string>

using namespace ci;

class NewParticleDrawer : public ParticleDrawerInterface
{
public:
  NewParticleDrawer (const string& vertexShaderName="pointsprite_es3.vert", const string& fragmentShaderName="pointsprite_es3.frag");
  virtual ~NewParticleDrawer ();

  // ParticleDrawerInterface operations
  void setup (const vector<Particle> &particles) override;
  virtual void beforeDraw () override;
  void drawParticles (const vector<Particle> &particles, const size_t nofParticles, const vec2 &textureSize) override;
  void afterDraw () override;
  
  void myDrawBillboard( const vec3 &pos, const vec2 &scale, float rotationRadians, const vec3 &bbRight, const vec3 &bbUp, const Rectf &texCoords = Rectf (0, 0, 1, 1));
  
protected:
  string          m_vertexShaderName;
  string          m_fragmentShaderName;
  
  gl::BatchRef		m_particleBatch;
  gl::VboRef			m_particleVbo;
};
