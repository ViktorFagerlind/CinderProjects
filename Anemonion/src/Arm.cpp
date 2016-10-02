#include "Arm.h"

#include "BSpline.h"
#include "MiscMath.h"
#include "TubeDrawer.h"

#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include "cinder/CinderMath.h"

Arm::Arm (const vec3&    startNormal, 
          const float     startLength,
          const uint32_t  nofSegmentsPerJoint, 
          const uint32_t  nofJoints, 
          const float     jointLength,
          const float     radius)
: m_nofSegmentsPerJoint (nofSegmentsPerJoint),
  m_radius              (radius),
  m_position            (vec3(0,0,0)),
  m_startNormal         (startNormal)
{
  // Create initial joints
  m_Joints.push_back (Joint (m_position, m_startNormal, startLength));
  for (uint32_t i=1; i<nofJoints; i++)
  {
    m_Joints.push_back (Joint (m_Joints[i-1].getEndPosition (),
                               (m_Joints[i-1].m_normal*0.5f + vec3 (Rand::randFloat(-1,1), 
                                                                     Rand::randFloat(-1,1), 
                                                                     Rand::randFloat(-1,1))).normalized (), 
                               jointLength));
  }

  m_drawPoints.resize (m_nofSegmentsPerJoint * nofJoints);
}

void Arm::setRotation (const Matrix44<float>& rotationMatrix)
{
  m_rotation = rotationMatrix;
}

void Arm::setPosition (const vec3& position)
{
  m_position = position;
}

void Arm::update ()
{
  // Note that the first joint is never updated, it is stuck on the body

  m_Joints[0].m_position = Matrix44<float>::createTranslation (m_position) * m_rotation * vec3(0,0,0);
  m_Joints[0].m_normal   = m_rotation * m_startNormal;

  for (uint32_t i=1; i<m_Joints.size (); i++)
    m_Joints[i].update (m_Joints[i-1].getEndPosition (), m_Joints[i-1].m_normal);

}

	
void Arm::draw (gl::GlslProg& shader)
{
  // Create all the points along the b-spline joints
  uint32_t nofPoints = 0;
  for (uint32_t i=0; i<m_Joints.size () - 3; i++)
  {
    for (uint32_t j=0; j<m_nofSegmentsPerJoint; j++)
    {
      m_drawPoints[nofPoints] = VfBSpline::calc3D (m_Joints[i  ].m_position, 
                                                   m_Joints[i+1].m_position, 
                                                   m_Joints[i+2].m_position, 
                                                   m_Joints[i+3].m_position, 
                                                   (float) j / (float) m_nofSegmentsPerJoint);
      nofPoints++;
    }
  }

  m_normals.resize (nofPoints-1);
  m_radie.resize   (nofPoints-1);

  m_radie[0]   = VfBSpline::calc1D (m_radius*10.f, m_radius, 0.f, 0.f, 0.f);
  m_normals[0] = (m_drawPoints[1] - m_drawPoints[0]).normalized ();

  for (uint32_t i=0; i<nofPoints - 2; i++)
  {
    vec3 currentToNext  = m_drawPoints[i+1] - m_drawPoints[i];
    vec3 nextToNextNext = m_drawPoints[i+2] - m_drawPoints[i+1];

    m_radie[i+1]   = VfBSpline::calc1D (m_radius*5.f, m_radius, 0.f, 0.f, (float)(i+(float)nofPoints*0.2f)/((float)nofPoints*1.2f));
    m_normals[i+1] = (currentToNext + nextToNextNext).normalized ();
  }

  // Choose a general direction for the "up" vector so that it is perpendicular to the 
  // general layout of the entire line, in that way the face normals are less likely 
  // to be aligned to the direction (which is not good when projecting on that plane)
  vec3 upDirection = vec3 (0,0,1).cross (m_Joints[m_Joints.size()-1].m_position - m_Joints[0].m_position);

  TubeDrawer::draw (shader, 
                    upDirection,
                    true,
                    8,
                    m_drawPoints.data (),
                    m_normals.data (),
                    m_radie.data (),
                    nofPoints - 1);

  
#ifdef _DEBUG
  shader.unbind ();

  for (uint32_t i=0; i<m_Joints.size () - 1; i++)
  {
    gl::color (1,1,0);
    gl::drawLine (m_Joints[i].m_position, m_Joints[i+1].m_position);
  }

  TubeDrawer::debugDraw (upDirection,
                         m_drawPoints.data (), 
                         m_normals.data (), 
                         m_radie.data (),
                         nofPoints - 1);
  shader.bind ();
#endif
  
}
