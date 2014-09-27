#pragma once

#include "Straw.h"

#include "PhongMaterial.h"

#include "cinder/Perlin.h"
#include "cinder/Vector.h"
#include "cinder/gl/Vbo.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/Matrix44.h"
#include <vector>

using std::list;

using namespace ci;

class Grass
{
public:
  class Grass (const float radius, const Vec3f& direction);
  
  void rotate (const Matrix44<float>& rotationMatrix);

  void move   (const Vec3f& offset);
	
  void update ();
	
	void draw ();

private:
	void animate ();

private:
  const float                       m_radius;

  shared_ptr<PhongMaterial>         m_material;
  std::vector<shared_ptr<Straw>>    m_straws;

  Matrix44<float>                   m_rotation;

  Perlin                            m_perlin;
  float                             m_animationCounter;
};

