#pragma once

#include "cinder/Vector.h"
#include "cinder/gl/GlslProg.h"
#include <vector>

using std::list;

using namespace ci;

class Joint
{
public:
  Joint (const Vec3f& normal, const float length)
  : m_normal (normal),
    m_length (length)
  {
  }

public:
  Vec3f m_normal;
  float m_length;
  Vec3f m_pos;
};

class Tube
{
public:
  class Tube (const Vec3f& startPosition, const Vec3f& startNormal);
  
  void update ();
	
	void draw (gl::GlslProg& shader);

private:

  void setJointPositions ();

	void drawSegment (const Vec3f&  point1, 
                    const Vec3f&  point2,
                    const Vec3f&  planeNormal1, 
                    const Vec3f&  planeNormal2,
                    const Vec3f&  upDirection,
                    gl::GlslProg& shader);

private:

  static const uint32_t m_nofSplineSegments;

  std::vector<Joint> m_Joints;

  std::vector<Vec3f> m_drawPoints;
};

