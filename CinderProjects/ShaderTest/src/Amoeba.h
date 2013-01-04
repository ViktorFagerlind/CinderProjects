#pragma once

#include "Tube.h"

#include "PhongMaterial.h"

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
  class Amoeba (const float radius);
  
  void rotate (const Matrix44<float>& rotationMatrix);
	
  void update ();
	
	void draw ();

private:
  const float                     m_radius;

  shared_ptr<PhongMaterial>       m_tubeMaterial;
  shared_ptr<PhongMaterial>       m_bodyMaterial;
  std::vector<shared_ptr<Tube>>   m_tubes;

  gl::VboMesh                     m_bodyMesh;
};

