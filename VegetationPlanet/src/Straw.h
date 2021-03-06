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
  Joint (const vec3& position, const vec3& normal, const float length)
  : m_position (position),
    m_normal (normal),
    m_length (length)
  {
  }

  void update (const vec3& newPosition, const vec3& wantedNormal)
  {
    // Update normal
    vec3 newTempNormal = (getEndPosition () - newPosition).normalized ();
    float angle = math<float>::acos (wantedNormal.dot (newTempNormal));

    if (angle > 5.f * (float)M_PI / 180.f) // only rotate if angle is greater than threashold
    {
      m_normal = newTempNormal;

      vec3 rotationAxis = newTempNormal.cross (wantedNormal);
      m_normal.rotate (rotationAxis, angle*0.3f);

      m_normal.normalize ();
    }

    // Update position
    m_position = newPosition;
  }

  vec3 getEndPosition ()
  {
    return m_position + m_length * m_normal;
  }

public:
  vec3       m_normal;
  const float m_length;
  vec3       m_position;
};

class Straw
{
public:
  class Straw (const vec3&    startNormal, 
              const vec3&    position,
              const uint32_t  nofCircularSegments, 
              const uint32_t  nofSegmentsPerJoint, 
              const uint32_t  nofJoints, 
              const float     jointLength,
              const float     radius);
  
  void setRotation (const Matrix44<float>& rotationMatrix);

  void setPosition (const vec3& position);
	
  void update ();
	
	void draw (gl::GlslProg& shader);

  const vec3& getStartNormal () {return m_Joints[0].m_normal;}

private:

	void drawSegment (const vec3&  point1, 
                    const vec3&  point2,
                    const vec3&  planeNormal1, 
                    const vec3&  planeNormal2,
                    const float   radius1, 
                    const float   radius2,
                    const vec3&  upDirection,
                    gl::GlslProg& shader);

private:
  const uint32_t m_nofCircularSegments;
  const uint32_t m_nofSegmentsPerJoint;
  const float    m_radius;

  std::vector<Joint> m_Joints;

  std::vector<vec3> m_drawPoints;
  std::vector<vec3> m_normals;
  std::vector<float> m_radie;

  vec3              m_position;
  vec3              m_startNormal;

  Matrix44<float>    m_rotation;
};

