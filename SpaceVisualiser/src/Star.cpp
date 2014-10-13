#include "Star.h"
#include "ShaderHelper.h"
#include "MiscMath.h"

#include "ParticleSystem.h"
#include "TubeParticleDrawer.h"
#include "PointEmitter.h"

#include "cinder/Rand.h"
#include "cinder/ObjLoader.h"

Star::Star (ParticleSystem *particleSystem, const float radius, const Vec3f position)
: m_radius (radius),
  m_position (position)
{
  ColorAf ambient   = ColorAf (0.05f, 0.05f, 0.15f, 1.f) + ColorAf (Rand::randFloat (-0.3f, 0.3f), Rand::randFloat (-0.3f, 0.3f), 0, 0);
  ColorAf diffuse   = ColorAf (0.55f, 0.35f, 0.50f, 1.f) + ColorAf (Rand::randFloat (-0.3f, 0.3f), Rand::randFloat (-0.3f, 0.3f), 0, 0);
  ColorAf specular  = ColorAf (0.80f, 0.70f, 0.90f, 1.f) + ColorAf (Rand::randFloat (-0.3f, 0.3f), Rand::randFloat (-0.3f, 0.3f), 0, 0);
  float   shininess = Rand::randFloat (5.f, 20.f);

  // Load mesh
  ObjLoader    loader (loadFile ("../Media/Meshes/Sphere.obj"));
  TriMesh      bodyTriMesh;
  loader.load (&bodyTriMesh);
	m_bodyMesh = gl::VboMesh (bodyTriMesh);

	m_perlin.setSeed (clock ());
  m_animationCounter = 0.f;

  gl::GlslProg shader = ShaderHelper::loadShader ("../Media/Shaders/phong_vert.glsl",
                                                  "../Media/Shaders/tube_frag.glsl");

  m_bodyMaterial.reset (new PhongMaterial (shader, ambient, diffuse, specular, shininess));


  shared_ptr<TubeParticleDrawer> tubeDrawer = shared_ptr<TubeParticleDrawer> (new TubeParticleDrawer (20, 1, ambient, diffuse, specular, shininess));

  m_emitter = new PointEmitter (50000,
                                Vec3f (0, 0, 0),       // position 
                                6.0f,                  // particles per frame
                                .1f,                   // min size
                                .1f,                   // max size
                                Vec3f (0, 0, 0),       // baseVelocity
                                1.f,                   // minRandVelocity
                                2.f,                   // maxRandVelocity
                                tubeDrawer);

  particleSystem->addEmitter (m_emitter);

}

void Star::rotate (const Matrix44<float>& rotationMatrix)
{
  m_rotation *= rotationMatrix;
}

void Star::move (const Vec3f& offset)
{
  m_position += offset;
}

void Star::animate ()
{
  const float speed           = 1.f;
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

  m_animationCounter += 1.f;
}

void Star::bounce ()
{
  m_emitter->burst (300);
}

void Star::update ()
{
  animate ();

  m_emitter->setPosition (m_position);
}

void Star::draw ()
{
  m_bodyMaterial->bind ();

  gl::pushModelView ();

  gl::translate (m_position);
  gl::multModelView (m_rotation);

  gl::enable  (GL_RESCALE_NORMAL);
  gl::scale   (m_radius, m_radius, m_radius);

//  gl::draw    (m_bodyMesh);
  gl::disable (GL_RESCALE_NORMAL);

  gl::popModelView ();

  m_bodyMaterial->unbind ();
}
