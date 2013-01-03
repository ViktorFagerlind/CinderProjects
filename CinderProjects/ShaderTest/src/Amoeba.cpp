#include "Amoeba.h"
#include "ShaderHelper.h"

#include "cinder/Rand.h"
#include "cinder/ObjLoader.h"

Amoeba::Amoeba ()
{
  ColorAf ambient  (0.05f, 0.1f,  0.05f, 1.0f);
  ColorAf diffuse  (0.8f,  0.5f,  0.2f,  1.0f);
  ColorAf specular (1.0f,  0.7f,  0.6f,  1.0f);
  float   shininess = 10.0f;
  gl::GlslProg shader;

  shader = ShaderHelper::loadShader ("../Media/Shaders/tube_vert.glsl", 
                                     "../Media/Shaders/tube_frag.glsl",
                                     "../Media/Shaders/tube_geom.glsl",
                                     GL_POINTS,
                                     GL_TRIANGLE_STRIP,
                                     1024);
  m_tubeMaterial.reset (new PhongMaterial (shader, ambient, diffuse, specular, shininess));

  shader = ShaderHelper::loadShader ("../Media/Shaders/phong_vert.glsl", 
                                     "../Media/Shaders/phong_frag.glsl");
  m_bodyMaterial.reset (new PhongMaterial (shader, ambient, diffuse, specular, shininess));


  for (uint32_t i=0; i<20; i++)
  {
    shared_ptr<Tube> t(new Tube (Vec3f(0,0,0), Vec3f (Rand::randFloat(-1,1), 
                                                      Rand::randFloat(-1,1), 
                                                      Rand::randFloat(-1,1)).normalized ()));
    m_tubes.push_back (t);
  }

  // Load mesh
  ObjLoader    loader (loadFile ("../Media/Meshes/Sphere.obj"));
  TriMesh      bodyTriMesh;
  loader.load (&bodyTriMesh);
	m_bodyMesh = gl::VboMesh (bodyTriMesh);
}

void Amoeba::update ()
{
}

void Amoeba::draw ()
{
  m_bodyMaterial->bind ();
  gl::draw (m_bodyMesh);
  m_bodyMaterial->unbind ();

  m_tubeMaterial->bind ();

  for (uint32_t i=0; i<m_tubes.size (); i++)
    m_tubes[i]->draw (m_tubeMaterial->getShader ());

  m_tubeMaterial->unbind ();
}
