#include "Grass.h"
#include "ShaderHelper.h"
#include "MiscMath.h"
#include "VegetationMaterials.h"

#include "cinder/Rand.h"
#include "cinder/ObjLoader.h"

Grass::Grass (const float radius, const Vec3f& direction)
: m_radius (radius)
{

  m_material = VegetationMaterials::getSingleton ().getGrassMaterial ();

  for (uint32_t i=0; i<20; i++)
  {
    Vec3f grassDirection = (direction + MiscMath::getRandomDirection ()*Rand::randFloat (.2f,.4f)).normalized ();

    shared_ptr<Straw>  t(new Straw (grassDirection,                         // Start normal
                                    grassDirection * m_radius * 0.9f,      // Position
                                    3,                                      // Number of circle segments
                                    2,                                      // Number of segments per joint
                                    5,                                      // Number of joint
                                    Rand::randFloat (1.2f, 2.0f),           // Joint lengths
                                    Rand::randFloat (.05f, 0.2f)));         // VegTube radius
    m_straws.push_back (t);
  }


	m_perlin.setSeed (clock ());
  m_animationCounter = 0.f;
}

void Grass::rotate (const Matrix44<float>& rotationMatrix)
{
  m_rotation *= rotationMatrix;
}

void Grass::move (const Vec3f& offset)
{
//  m_position += offset;
}

void Grass::animate ()
{
/*
  const float rotationsSpeed  = 0.001f;

	Vec3f rotation  = m_perlin.dfBm (Vec3f ((m_position.x + m_animationCounter/2.f) * 0.005f, 
                                          (m_position.y + m_animationCounter/2.f) * 0.005f,
                                          (m_position.z + m_animationCounter/2.f) * 0.005f));

  rotate (Matrix44<float>::createRotation (rotation * rotationsSpeed));

  m_animationCounter += 0.02f;
*/
}

void Grass::update ()
{
  for (uint32_t i=0; i<m_straws.size (); i++)
  {
    animate ();

    m_straws[i]->setRotation (m_rotation);
//    m_straws[i]->setPosition (m_position);

    m_straws[i]->update ();
  }
}

void Grass::draw ()
{
  m_material->bind ();

  for (uint32_t i=0; i<m_straws.size (); i++)
    m_straws[i]->draw (m_material->getShader ());

  m_material->unbind ();
}
