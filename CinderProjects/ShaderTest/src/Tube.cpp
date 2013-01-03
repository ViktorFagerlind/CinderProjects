#include "Tube.h"

#include "BSpline.h"

#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

#define DEBUG

const uint32_t Tube::m_nofSplineSegments = 10UL; // how many points per spline section

Tube::Tube ()
{
  // Create initial joints
  m_Joints.push_back (Joint (Vec3f (1, 0, 0), 10.f));
  for (uint32_t i=0; i<4; i++)
  {
    m_Joints.push_back (Joint (Vec3f (Rand::randFloat(-1,1), 
                                      Rand::randFloat(-1,1), 
                                      Rand::randFloat(-1,1)).normalized (), 10.f));
  }

  m_drawPoints.resize (m_nofSplineSegments);
}

void Tube::update()
{
}

#ifdef DEBUG
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
#ifdef DEBUG
    Vec3f up1, side1;

    up1          = projectOnPlane (planeNormal1, upDirection).normalized ();
    side1        = planeNormal1.cross (up1).normalized();

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

	
void Tube::draw (gl::GlslProg& shader)
{
  Vec3f jointPoints[4];

  // Setup joint positions
  jointPoints[0] = Vec3f (0,0,0);
  for (uint32_t i=1; i<4; i++)
    jointPoints[i] = jointPoints[i-1] + m_Joints[i-1].m_length * m_Joints[i-1].m_normal;

  for (uint32_t i=0; i<m_nofSplineSegments; i++)
  {
    m_drawPoints[i] = VfBSpline::calc3D (jointPoints[0], jointPoints[1], jointPoints[2], jointPoints[3], (float) i / (float) m_nofSplineSegments);
  }

  // Choose a general direction for the "up" vector so that it is perpendicular to the 
  // general layout of the entire line, in that way the face normals are less likely 
  // to be aligned to the direction (which is not good when projecting on that plane)
  Vec3f upDirection = Vec3f (0,0,1).cross (jointPoints[m_Joints.size()-1] - jointPoints[0]);

  Vec3f planeNormal1;
  Vec3f planeNormal2;

  for (uint32_t i=0; i<m_nofSplineSegments - 2; i++)
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
