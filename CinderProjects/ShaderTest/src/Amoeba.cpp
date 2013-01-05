#include "Amoeba.h"
#include "ShaderHelper.h"
#include "MiscMath.h"

#include "cinder/Rand.h"
#include "cinder/ObjLoader.h"

Amoeba::Amoeba (const float radius)
: m_radius (radius)
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


  for (uint32_t i=0; i<30; i++)
  {
    Vec3f direction;
    
    if (i==0) 
      direction = Vec3f(1,0,0);
    else // Do not allow new tube to close to another
    {
      bool found = false;
      while (!found)
      {
        found = true;
        direction = MiscMath::getRandomDirection ();
        for (uint32_t j=0; j<i && found; j++)
        {
          float angle = math<float>::acos (m_tubes[j]->getStartNormal ().dot (direction));
          if (angle < 30.f * (float)M_PI / 180.f)
            found = false;
        }
      }
    }

    shared_ptr<Tube>  t(new Tube (Vec3f(0,0,0),  // Start position
                                  direction,     // Start normal
                                  m_radius,      // First segment length
                                  5,             // Number of segments per joint
                                  8,             // Number of joint
                                  3,             // Segment lengths
                                  0.4f));        // Tube radius
    m_tubes.push_back (t);
  }

  // Load mesh
  ObjLoader    loader (loadFile ("../Media/Meshes/Sphere.obj"));
  TriMesh      bodyTriMesh;
  loader.load (&bodyTriMesh);
	m_bodyMesh = gl::VboMesh (bodyTriMesh);
}

void Amoeba::rotate (const Matrix44<float>& rotationMatrix)
{
  for (uint32_t i=0; i<m_tubes.size (); i++)
    m_tubes[i]->rotate (rotationMatrix);
}

void Amoeba::update ()
{
  for (uint32_t i=0; i<m_tubes.size (); i++)
    m_tubes[i]->update ();
}

void Amoeba::draw ()
{
  gl::enableDepthRead ();
  gl::enableDepthWrite ();

  m_bodyMaterial->bind ();

  gl::pushModelView ();
  gl::enable  (GL_RESCALE_NORMAL);
  gl::scale   (m_radius, m_radius, m_radius);
  gl::draw    (m_bodyMesh);
  gl::disable (GL_RESCALE_NORMAL);

  gl::popModelView ();

  m_bodyMaterial->unbind ();


  m_tubeMaterial->bind ();

  for (uint32_t i=0; i<m_tubes.size (); i++)
    m_tubes[i]->draw (m_tubeMaterial->getShader ());

  m_tubeMaterial->unbind ();
}
