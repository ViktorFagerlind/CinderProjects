#pragma once

#include "PhongMaterial.h"

#include "cinder/Perlin.h"
#include "cinder/Vector.h"
#include "cinder/gl/Vbo.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/Matrix44.h"
#include <vector>

using namespace std;

using namespace ci;

class ParticleSystem;
class PointEmitter;
class TubeParticleDrawer;

class Star
{
public:
  class Star (ParticleSystem *particleSystem, const float radius, const vec3 position = vec3 (0, 0, 0));
  
  void rotate (const Matrix44<float>& rotationMatrix);

  void move (const vec3& offset);

  void setBands (const vector<float>& bands);

  const vec3& getPosition () { return m_position; }
	
  void bounce ();

  void update ();

  void draw ();

private:
	void animate ();

private:
  ColorAf                         m_originalDiffuse;

  shared_ptr<TubeParticleDrawer> m_tubeDrawer;

  PointEmitter                   *m_emitter;

  const float                     m_radius;

  gl::VboMesh                     m_bodyMesh;

  vec3                           m_position;

  Matrix44<float>                 m_rotation;

  Perlin                          m_perlin;

  shared_ptr<PhongMaterial>       m_bodyMaterial;

  float                           m_animationCounter;

};

