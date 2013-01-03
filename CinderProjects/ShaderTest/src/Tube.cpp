#include "Tube.h"

#include "BSpline.h"

#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

const uint32_t Tube::m_nofSplineSegments = 10UL; // how many points per spline section

Tube::Tube (const Vec3f& startPosition, const Vec3f& startNormal)
{
  const float segmentLength = 4.f;

  // Create initial joints
  m_Joints.push_back (Joint (startNormal, segmentLength));
  for (uint32_t i=1; i<20; i++)
  {
    m_Joints.push_back (Joint ((m_Joints[i-1].m_normal*1.5f + Vec3f (Rand::randFloat(-1,1), 
                                                                     Rand::randFloat(-1,1), 
                                                                     Rand::randFloat(-1,1))
                               ).normalized (), segmentLength));
  }

  m_Joints[0].m_pos = startPosition;

  m_drawPoints.resize (m_nofSplineSegments * m_Joints.size ());
}

void Tube::update()
{
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
                        const Vec3f&  upDirection,
                        gl::GlslProg& shader)
{
#ifdef _DEBUG
    Vec3f up1, side1;

    up1   = projectOnPlane (planeNormal1, upDirection).normalized ();
    side1 = planeNormal1.cross (up1).normalized();

    shader.unbind ();
    gl::color (1,0,0);
    gl::drawVector (point1*0.5f, (point1 + planeNormal1)*0.5f);
    gl::color (0,1,0);
    gl::drawVector (point1*0.5f, (point1 + up1)*0.5f);
    gl::color (0,0,1);
    gl::drawVector (point1*0.5f, (point1 + side1)*0.5f);
    shader.bind ();
#endif


    shader.uniform ("u_generalUp",    upDirection);

    shader.uniform ("u_point2",       point2);
    shader.uniform ("u_planeNormal1", planeNormal1);
    shader.uniform ("u_planeNormal2", planeNormal2);

    gl::begin  (GL_POINTS);
    gl::vertex (point1);
    gl::end    ();
}

void Tube::setJointPositions ()
{
  for (uint32_t i=1; i<m_Joints.size (); i++)
    m_Joints[i].m_pos = m_Joints[i-1].m_pos + m_Joints[i-1].m_length * m_Joints[i-1].m_normal;
}
	
void Tube::draw (gl::GlslProg& shader)
{
  setJointPositions ();

  // Choose a general direction for the "up" vector so that it is perpendicular to the 
  // general layout of the entire line, in that way the face normals are less likely 
  // to be aligned to the direction (which is not good when projecting on that plane)
  Vec3f upDirection = Vec3f (0,0,1).cross (m_Joints[m_Joints.size()-1].m_pos - m_Joints[0].m_pos);

  // Create all the points along the b-spline joints
  uint32_t nofPoints = 0;
  for (uint32_t i=0; i<m_Joints.size () - 3; i++)
  {
    for (uint32_t j=0; j<m_nofSplineSegments; j++)
    {
      m_drawPoints[nofPoints] = VfBSpline::calc3D (m_Joints[i  ].m_pos, 
                                                   m_Joints[i+1].m_pos, 
                                                   m_Joints[i+2].m_pos, 
                                                   m_Joints[i+3].m_pos, 
                                                   (float) j / (float) m_nofSplineSegments);
      nofPoints++;
    }
  }

#ifdef _DEBUG
  shader.unbind ();
  for (uint32_t i=0; i<m_Joints.size () - 1; i++)
  {
    gl::color (1,1,0);
    gl::drawVector (m_Joints[i].m_pos*0.5f, m_Joints[i+1].m_pos*0.5f);
  }
  shader.bind ();
#endif

  // Draw the tube along the b-spline route
  Vec3f planeNormal1;
  Vec3f planeNormal2;

  for (uint32_t i=0; i<nofPoints - 2; i++)
  {
    Vec3f currentToNext  = m_drawPoints[i+1] - m_drawPoints[i];
    Vec3f nextToNextNext = m_drawPoints[i+2] - m_drawPoints[i+1];

    if (i==0)
      planeNormal1 = currentToNext.normalized ();
    else
      planeNormal1 = planeNormal2;

    planeNormal2 = (currentToNext + nextToNextNext).normalized ();

    drawSegment (m_drawPoints[i], 
                 m_drawPoints[i+1],
                 planeNormal1, 
                 planeNormal2,
                 upDirection,
                 shader);
  }
}
