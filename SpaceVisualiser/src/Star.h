#pragma once

#include "PhongMaterial.h"

#include "cinder/Perlin.h"
#include "cinder/Vector.h"
#include "cinder/gl/Vbo.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/Matrix44.h"
#include <vector>

using std::list;

using namespace ci;

class ParticleSystem;
class PointEmitter;

class Star
{
public:
  class Star (ParticleSystem *particleSystem, const float radius, const Vec3f position = Vec3f (0, 0, 0));
  
  void rotate (const Matrix44<float>& rotationMatrix);

  void move   (const Vec3f& offset);

  const Vec3f& getPosition () {return m_position;}
	
  void bounce ();

  void update ();

  void draw ();

private:
	void animate ();

private:
  PointEmitter                   *m_emitter;

  const float                     m_radius;

  gl::VboMesh                     m_bodyMesh;

  Vec3f                           m_position;

  Matrix44<float>                 m_rotation;

  Perlin                          m_perlin;

  shared_ptr<PhongMaterial>       m_bodyMaterial;

  float                           m_animationCounter;

};

