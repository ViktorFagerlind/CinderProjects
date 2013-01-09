#include "Tube.h"

#include "BSpline.h"

#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include "cinder/CinderMath.h"

Tube::Tube (const Vec3f&    startNormal, 
            const float     startLength,
            const uint32_t  nofSegmentsPerJoint, 
            const uint32_t  nofJoints, 
            const float     jointLength,
            const float     radius)
: m_nofSegmentsPerJoint (nofSegmentsPerJoint),
  m_radius              (radius),
  m_position            (Vec3f(0,0,0)),
  m_startNormal         (startNormal)
{
  // Create initial joints
  m_Joints.push_back (Joint (m_position, m_startNormal, startLength));
  for (uint32_t i=1; i<nofJoints; i++)
  {
    m_Joints.push_back (Joint (m_Joints[i-1].getEndPosition (),
                               (m_Joints[i-1].m_normal*0.5f + Vec3f (Rand::randFloat(-1,1), 
                                                                     Rand::randFloat(-1,1), 
                                                                     Rand::randFloat(-1,1))).normalized (), 
                               jointLength));
  }

  m_drawPoints.resize (m_nofSegmentsPerJoint * nofJoints);
}

void Tube::setRotation (const Matrix44<float>& rotationMatrix)
{
  m_rotation = rotationMatrix;
}

void Tube::setPosition (const Vec3f& position)
{
  m_position = position;
}

void Tube::update ()
{
  // Note that the first joint is never updated, it is stuck on the body

  m_Joints[0].m_position = Matrix44<float>::createTranslation (m_position) * m_rotation * Vec3f(0,0,0);
  m_Joints[0].m_normal   = m_rotation * m_startNormal;

  for (uint32_t i=1; i<m_Joints.size (); i++)
    m_Joints[i].update (m_Joints[i-1].getEndPosition (), m_Joints[i-1].m_normal);

}

#ifdef _DEBUG
// Projects v onto a plane with normal n
// n must be of unity length
Vec3f projectOnPlane (const Vec3f& n, const Vec3f& v)
{
  return v - n * n.dot (v);
}
#endif

void Tube::drawSegment (const Vec3f&  point1, 
                        const Vec3f&  point2,
                        const Vec3f&  planeNormal1, 
                        const Vec3f&  planeNormal2,
                        const float   radius1, 
                        const float   radius2,
                        const Vec3f&  upDirection,
                        gl::GlslProg& shader)
{
#ifdef _DEBUG
    Vec3f up1, side1;

    up1   = projectOnPlane (planeNormal1, upDirection).normalized ();
    side1 = planeNormal1.cross (up1).normalized();

    shader.unbind ();
    gl::color (1,0,0);
    gl::drawLine (point1, point1 + planeNormal1);
    gl::color (0,1,0);
    gl::drawLine (point1, point1 + up1);
    gl::color (0,0,1);
    gl::drawLine (point1, point1 + side1);
    shader.bind ();
#endif

    shader.uniform ("u_point2",       point2);
    shader.uniform ("u_planeNormal1", planeNormal1);
    shader.uniform ("u_planeNormal2", planeNormal2);
    shader.uniform ("u_radius1",      radius1);
    shader.uniform ("u_radius2",      radius2);

    gl::begin  (GL_POINTS);
    gl::vertex (point1);
    gl::end    ();
}
	
void Tube::draw (gl::GlslProg& shader)
{
  // Choose a general direction for the "up" vector so that it is perpendicular to the 
  // general layout of the entire line, in that way the face normals are less likely 
  // to be aligned to the direction (which is not good when projecting on that plane)
  Vec3f upDirection = Vec3f (0,0,1).cross (m_Joints[m_Joints.size()-1].m_position - m_Joints[0].m_position);

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

#ifdef _DEBUG
  shader.unbind ();
  for (uint32_t i=0; i<m_Joints.size () - 1; i++)
  {
    gl::color (1,1,0);
    gl::drawLine (m_Joints[i].m_position, m_Joints[i+1].m_position);
  }
  shader.bind ();
#endif

  // Draw the tube along the b-spline route
  Vec3f planeNormal1;
  Vec3f planeNormal2;
  float radius1;
  float radius2;

  shader.uniform ("u_generalUp",   upDirection);
  shader.uniform ("u_nofSegments", 10);

  for (uint32_t i=0; i<nofPoints - 2; i++)
  {
    Vec3f currentToNext  = m_drawPoints[i+1] - m_drawPoints[i];
    Vec3f nextToNextNext = m_drawPoints[i+2] - m_drawPoints[i+1];

    if (i==0)
    {
      shader.uniform ("u_blendSphere", true);
      radius1      = VfBSpline::calc1D (m_radius*10.f, m_radius, 0.f, 0.f, 0.f);
      planeNormal1 = currentToNext.normalized ();
    }
    else
    {
      shader.uniform ("u_blendSphere", false);
      radius1      = radius2;
      planeNormal1 = planeNormal2;
    }

    radius2      = VfBSpline::calc1D (m_radius*5.f, m_radius, 0.f, 0.f, (float)(i+(float)nofPoints*0.2f)/((float)nofPoints*1.2f));
    planeNormal2 = (currentToNext + nextToNextNext).normalized ();

    drawSegment (m_drawPoints[i], 
                 m_drawPoints[i+1],
                 planeNormal1, 
                 planeNormal2,
                 radius1,
                 radius2,
                 upDirection,
                 shader);
  }
}
