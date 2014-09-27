#pragma once

#include "PhongMaterial.h"

#include "cinder/gl/GlslProg.h"

using namespace ci;

class VegetationMaterials
{
public:
  static VegetationMaterials& getSingleton ();

  VegetationMaterials ();

  shared_ptr<PhongMaterial> getGrassMaterial ();

  shared_ptr<PhongMaterial> getBranchMaterial ();

  shared_ptr<PhongMaterial> getLeafMaterial ();

  shared_ptr<PhongMaterial> getFlowerMaterial ();

private:
  gl::GlslProg  m_tubeShader;

  gl::GlslProg  m_phongShader;
};

