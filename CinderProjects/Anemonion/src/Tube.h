#pragma once

#include "cinder/Vector.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/Matrix44.h"
#include <vector>

using std::list;

using namespace ci;

class Joint
{
public:
  Joint (const Vec3f& position, const Vec3f& normal, const float length)
  : m_position (position),
    m_normal (normal),
    m_length (length)
  {
  }

  void update (const Vec3f& newPosition, const Vec3f& wantedNormal)
  {
    // Update normal
    Vec3f newTempNormal = (getEndPosition () - newPosition).normalized ();
    float angle = math<float>::acos (wantedNormal.dot (newTempNormal));

    if (angle > 5.f * (float)M_PI / 180.f) // only rotate if angle is greater than threashold
    {
      m_normal = newTempNormal;

      Vec3f rotationAxis = newTempNormal.cross (wantedNormal);
      m_normal.rotate (rotationAxis, angle*0.3f);

      m_normal.normalize ();
    }

    // Update position
    m_position = newPosition;
  }

  Vec3f getEndPosition ()
  {
    return m_position + m_length * m_normal;
  }

public:
  Vec3f       m_normal;
  const float m_length;
  Vec3f       m_position;
};

class Tube
{
public:
  class Tube (const Vec3f&    startNormal, 
              const float     startLength,
              const uint32_t  nofSegmentsPerJoint, 
              const uint32_t  nofJoints, 
              const float     jointLength,
              const float     radius);
  
  void setRotation (const Matrix44<float>& rotationMatrix);

  void setPosition (const Vec3f& position);
	
  void update ();
	
	void draw (gl::GlslProg& shader);

  const Vec3f& getStartNormal () {return m_Joints[0].m_normal;}

private:

  const uint32_t m_nofSegmentsPerJoint;
  const float    m_radius;

  std::vector<Joint> m_Joints;

  std::vector<Vec3f> m_drawPoints;
  std::vector<Vec3f> m_normals;
  std::vector<float> m_radie;

  Vec3f              m_position;
  Vec3f              m_startNormal;

  Matrix44<float>    m_rotation;
};

