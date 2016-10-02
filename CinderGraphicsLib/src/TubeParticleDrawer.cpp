#include "TubeParticleDrawer.h"

#include "TubeDrawer.h"
#include "BSpline.h"
#include "ShaderHelper.h"

TubeParticle::TubeParticle (uint32_t maxNofPoints, uint32_t segmentLength)
: Particle (),
  m_maxNofPoints(maxNofPoints),
  m_segmentLength(segmentLength)
{

  m_points.resize (m_maxNofPoints);
  m_normals.resize (m_maxNofPoints);
  m_radie.resize (m_maxNofPoints);
}

TubeParticle::~TubeParticle ()
{
}

void TubeParticle::define (const vec3& position, float size, vec3 velocity)
{
  Particle::define (position, size, velocity);

  m_nofPoints   = 0;
  m_updateCount = 0;

  m_points[0]   = mPosition;
  m_normals[0]  = -getVelocity ().normalized ();
  m_radie[0]    = mCurrentSize;

  m_points[1]   = m_points[0];
  m_normals[1]  = m_normals[0];
  m_radie[1]    = m_radie[0];

  m_nofPoints = 2;
}

void TubeParticle::update ()
{
  Particle::update ();
  
  if (m_updateCount == m_segmentLength)
  {
    if (m_nofPoints < m_maxNofPoints)
      m_nofPoints++;

    for (uint32_t i = m_nofPoints - 1; i >= 1; i--)
    {
      m_points[i]  = m_points[i - 1];
      m_normals[i] = m_normals[i - 1];
      m_radie[i]   = m_radie[i - 1];
    }

    m_updateCount = 0;
  }
  else
    m_updateCount++;

  m_points[0]  = mPosition;
  m_normals[0] = -getVelocity ().normalized ();
  m_radie[0]   = mCurrentSize;
}


TubeParticleDrawer::TubeParticleDrawer (uint32_t tubeMaxNofPoints,
                                        uint32_t tubeSegmentLength,
                                        ColorAf  ambient,
                                        ColorAf  diffuse,
                                        ColorAf  specular,
                                        float    shininess)
: m_tubeMaxNofPoints (tubeMaxNofPoints), 
  m_tubeSegmentLength (tubeSegmentLength)
{
  gl::GlslProg shader = ShaderHelper::loadShader ("../Media/Shaders/tube_vert.glsl",
                                                  "../Media/Shaders/tube_frag.glsl",
                                                  "../Media/Shaders/tube_geom.glsl",
                                                  GL_POINTS,
                                                  GL_TRIANGLE_STRIP,
                                                  1024);

  m_material.reset (new PhongMaterial (shader, ambient, diffuse, specular, shininess));
}

TubeParticleDrawer::~TubeParticleDrawer ()
{
}

void TubeParticleDrawer::createParticles (vector<Particle*> &particles)
{
  for (uint32_t i = 0; i < particles.size (); i++)
    particles[i] = new TubeParticle (m_tubeMaxNofPoints, m_tubeSegmentLength);
};

void TubeParticleDrawer::beforeDraw ()
{
}

void TubeParticleDrawer::drawParticle (const Particle &p, const vec2 &textureSize)
{
  TubeParticle *tp = (TubeParticle *)&p;

  ColorAf c = m_material->getDiffuse ();
  c.a = p.mColor.a;
  m_material->setDiffuse (c);

  m_material->bind ();

  if (tp->m_nofPoints >= 2)
  {
    // Choose a general direction for the "up" vector so that it is perpendicular to the 
    // general layout of the entire line, in that way the face normals are less likely 
    // to be aligned to the direction (which is not good when projecting on that plane)
    vec3 up = vec3 (0, 0, 1).cross (tp->m_points[tp->m_nofPoints - 1] - tp->m_points[0]).normalized ();

    TubeDrawer::draw (m_material->getShader (),
                      up,
                      false,
                      3,
                      tp->m_points.data (),
                      tp->m_normals.data (),
                      tp->m_radie.data (),
                      tp->m_nofPoints);
  }

  m_material->unbind ();

}

void TubeParticleDrawer::afterDraw ()
{
}
