#include "VegetationMaterials.h"

#include "cinder/Rand.h"

#include "ShaderHelper.h"

//----------------------------------------------------------------------------------------------------------------------

VegetationMaterials& VegetationMaterials::getSingleton ()
{
    static VegetationMaterials instance;

    return instance;
}


//----------------------------------------------------------------------------------------------------------------------

VegetationMaterials::VegetationMaterials ()
{
  m_tubeShader = ShaderHelper::loadShader ("../Media/Shaders/tube_vert.glsl", 
                                           "../Media/Shaders/phong_frag.glsl",
                                           "../Media/Shaders/tube_geom.glsl",
                                           GL_POINTS,
                                           GL_TRIANGLE_STRIP,
                                           1024);

  m_phongShader = ShaderHelper::loadShader ("../Media/Shaders/phong_vert.glsl", 
                                            "../Media/Shaders/phong_frag.glsl");
}

//----------------------------------------------------------------------------------------------------------------------

shared_ptr<PhongMaterial> VegetationMaterials::getGrassMaterial ()
{
  shared_ptr<PhongMaterial> material;

  ColorAf ambient  = ColorAf (0.05f, 0.15f,  0.05f, 1.f) + ColorAf (Rand::randFloat(-0.1f, 0.1f), Rand::randFloat(-0.1f, 0.1f), 0);
  ColorAf diffuse  = ColorAf (0.25f, 0.70f,  0.20f, 1.f) + ColorAf (Rand::randFloat(-0.1f, 0.1f), Rand::randFloat(-0.1f, 0.1f), 0);
  ColorAf specular = ColorAf (0.80f, 0.95f,  0.70f, 1.f) + ColorAf (Rand::randFloat(-0.1f, 0.1f), Rand::randFloat(-0.1f, 0.1f), 0);
  float   shininess = Rand::randFloat(5.f, 20.f);

  material.reset (new PhongMaterial (m_tubeShader, ambient, diffuse, specular, shininess));

  return material;
}

//----------------------------------------------------------------------------------------------------------------------

shared_ptr<PhongMaterial> VegetationMaterials::getBranchMaterial ()
{
  shared_ptr<PhongMaterial> material;

  ColorAf ambient   = ColorAf (0.10f, 0.04f,  0.02f, 1.f) + ColorAf (Rand::randFloat (-0.05f, 0.05f), Rand::randFloat (-0.05f, 0.05f), Rand::randFloat (-0.05f, 0.05f));
  ColorAf diffuse   = ColorAf (0.20f, 0.12f,  0.06f, 1.f) + ColorAf (Rand::randFloat (-0.05f, 0.05f), Rand::randFloat (-0.05f, 0.05f), Rand::randFloat (-0.05f, 0.05f));
  ColorAf specular  = ColorAf (0.40f, 0.30f,  0.25f, 1.f) + ColorAf (Rand::randFloat (-0.05f, 0.05f), Rand::randFloat (-0.05f, 0.05f), Rand::randFloat (-0.05f, 0.05f));
  float   shininess = 2.f;

  material.reset (new PhongMaterial (m_tubeShader, ambient, diffuse, specular, shininess));

  return material;
}

//----------------------------------------------------------------------------------------------------------------------

shared_ptr<PhongMaterial> VegetationMaterials::getLeafMaterial ()
{
  shared_ptr<PhongMaterial> material;

  ColorAf ambient   = ColorAf (0.05f, 0.10f,  0.05f, 1.f) + ColorAf (Rand::randFloat (-0.05f, 0.05f), Rand::randFloat (-0.05f, 0.05f), Rand::randFloat (-0.05f, 0.05f));
  ColorAf diffuse   = ColorAf (0.10f, 0.30f,  0.15f, 1.f) + ColorAf (Rand::randFloat (-0.05f, 0.05f), Rand::randFloat (-0.05f, 0.05f), Rand::randFloat (-0.05f, 0.05f));
  ColorAf specular  = ColorAf (0.50f, 0.90f,  0.70f, 1.f) + ColorAf (Rand::randFloat (-0.05f, 0.05f), Rand::randFloat (-0.05f, 0.05f), Rand::randFloat (-0.05f, 0.05f));
  float   shininess = 10.f;

  material.reset (new PhongMaterial (m_phongShader, ambient, diffuse, specular, shininess));

  return material;
}

//----------------------------------------------------------------------------------------------------------------------

shared_ptr<PhongMaterial> VegetationMaterials::getFlowerMaterial ()
{
  shared_ptr<PhongMaterial> material;

  ColorAf ambient   = ColorAf (0.25f, 0.25f,  0.20f, 1.f) + ColorAf (Rand::randFloat (-0.05f, 0.05f), Rand::randFloat (-0.05f, 0.05f), Rand::randFloat (-0.05f, 0.05f));
  ColorAf diffuse   = ColorAf (0.50f, 0.50f,  0.80f, 1.f) + ColorAf (Rand::randFloat (-0.05f, 0.05f), Rand::randFloat (-0.05f, 0.05f), Rand::randFloat (-0.05f, 0.05f));
  ColorAf specular  = ColorAf (0.80f, 0.80f,  1.00f, 1.f) + ColorAf (Rand::randFloat (-0.05f, 0.05f), Rand::randFloat (-0.05f, 0.05f), Rand::randFloat (-0.05f, 0.05f));
  float   shininess = 1.f;

  material.reset (new PhongMaterial (m_phongShader, ambient, diffuse, specular, shininess));

  return material;
}

