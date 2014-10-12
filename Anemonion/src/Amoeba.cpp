#include "Amoeba.h"
#include "ShaderHelper.h"
#include "MiscMath.h"

#include "cinder/Rand.h"
#include "cinder/ObjLoader.h"

Amoeba::Amoeba (const float radius, const Vec3f position)
: m_radius (radius),
  m_position (position)
{
  ColorAf ambient  = ColorAf (0.05f, 0.05f,  0.15f, 1.f) + ColorAf (Rand::randFloat(-0.1f, 0.1f), Rand::randFloat(-0.1f, 0.1f), 0, 0);
  ColorAf diffuse  = ColorAf (0.15f, 0.35f,  0.50f, .5f) + ColorAf (Rand::randFloat(-0.1f, 0.1f), Rand::randFloat(-0.1f, 0.1f), 0, 0);
  ColorAf specular = ColorAf (0.50f, 0.70f,  0.90f, 1.f) + ColorAf (Rand::randFloat(-0.1f, 0.1f), Rand::randFloat(-0.1f, 0.1f), 0, 0);
  float   shininess = Rand::randFloat(5.f, 20.f);
  gl::GlslProg shader;

  shader = ShaderHelper::loadShader ("../Media/Shaders/tube_vert.glsl", 
                                     "../Media/Shaders/amoeba_frag.glsl",
                                     "../Media/Shaders/tube_geom.glsl",
                                     GL_POINTS,
                                     GL_TRIANGLE_STRIP,
                                     1024);
  m_tubeMaterial.reset (new PhongMaterial (shader, ambient, diffuse, specular, shininess));

  shader = ShaderHelper::loadShader ("../Media/Shaders/phong_vert.glsl", 
                                     "../Media/Shaders/amoeba_frag.glsl");
  m_bodyMaterial.reset (new PhongMaterial (shader, ambient, diffuse, specular, shininess));

  uint32_t  nofJoints   = Rand::randInt (10,15);
  float     jointLength = m_radius * Rand::randFloat (0.9f, 1.7f);
  float     tubeRadius  = m_radius * Rand::randFloat (0.07f, 0.15f);

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
          float angle = math<float>::acos (m_arms[j]->getStartNormal ().dot (direction));
          if (angle < 25.f * (float)M_PI / 180.f)
            found = false;
        }
      }
    }

    shared_ptr<Arm>  t(new Arm (direction,                                    // Start normal
                                  m_radius*0.8f,                                // First segment length
                                  4,                                            // Number of segments per joint
                                  nofJoints,                                    // Number of joint
                                  jointLength * Rand::randFloat (.8f, 1.2f),    // Joint lengths
                                  tubeRadius  * Rand::randFloat (.8f, 1.2f)));  // Tube radius
    m_arms.push_back (t);
  }

  // Load mesh
  ObjLoader    loader (loadFile ("../Media/Meshes/Sphere.obj"));
  TriMesh      bodyTriMesh;
  loader.load (&bodyTriMesh);
	m_bodyMesh = gl::VboMesh (bodyTriMesh);


	m_perlin.setSeed (clock ());
  m_animationCounter = 0.f;
}

void Amoeba::rotate (const Matrix44<float>& rotationMatrix)
{
  m_rotation *= rotationMatrix;
}

void Amoeba::move (const Vec3f& offset)
{
  m_position += offset;
}

void Amoeba::animate ()
{
  const float speed           = 0.03f;
  const float gravityStrength = 0.002f;
  const float rotationsSpeed  = 0.001f;

	Vec3f direction = m_perlin.dfBm (Vec3f ((m_position.x + m_animationCounter) * 0.005f, 
                                          (m_position.y + m_animationCounter) * 0.005f,
                                          (m_position.z + m_animationCounter) * 0.005f));

  move ((direction - m_position * gravityStrength) * speed);

	Vec3f rotation  = m_perlin.dfBm (Vec3f ((m_position.x + m_animationCounter/2.f) * 0.005f, 
                                          (m_position.y + m_animationCounter/2.f) * 0.005f,
                                          (m_position.z + m_animationCounter/2.f) * 0.005f));

  rotate (Matrix44<float>::createRotation (rotation * rotationsSpeed));

  m_animationCounter += 0.02f;
}

void Amoeba::update ()
{
  for (uint32_t i=0; i<m_arms.size (); i++)
  {
    animate ();

    m_arms[i]->setRotation (m_rotation);
    m_arms[i]->setPosition (m_position);

    m_arms[i]->update ();
  }
}

void Amoeba::draw ()
{
  m_bodyMaterial->bind ();

  gl::pushModelView ();


  gl::translate (m_position);
  gl::multModelView (m_rotation);

  gl::enable  (GL_RESCALE_NORMAL);
  gl::scale   (m_radius, m_radius, m_radius);

  gl::draw    (m_bodyMesh);
  gl::disable (GL_RESCALE_NORMAL);

  gl::popModelView ();

  m_bodyMaterial->unbind ();


  m_tubeMaterial->bind ();

  for (uint32_t i=0; i<m_arms.size (); i++)
    m_arms[i]->draw (m_tubeMaterial->getShader ());

  m_tubeMaterial->unbind ();
}
