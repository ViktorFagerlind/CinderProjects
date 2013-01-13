#pragma once

#include "Arm.h"

#include "PhongMaterial.h"

#include "cinder/Perlin.h"
#include "cinder/Vector.h"
#include "cinder/gl/Vbo.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/Matrix44.h"
#include <vector>

using std::list;

using namespace ci;

class Amoeba
{
public:
  class Amoeba (const float radius, const Vec3f position = Vec3f(0,0,0));
  
  void rotate (const Matrix44<float>& rotationMatrix);

  void move   (const Vec3f& offset);

  const Vec3f& getPosition () {return m_position;}
	
  void update ();
	
	void draw ();

private:
	void animate ();

private:
  const float                     m_radius;

  shared_ptr<PhongMaterial>       m_tubeMaterial;
  shared_ptr<PhongMaterial>       m_bodyMaterial;
  std::vector<shared_ptr<Arm>>    m_arms;

  gl::VboMesh                     m_bodyMesh;

  Vec3f                           m_position;

  Matrix44<float>                 m_rotation;

  Perlin                          m_perlin;
  float                           m_animationCounter;
};

