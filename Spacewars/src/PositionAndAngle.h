#pragma once

#include "cinder/Vector.h"

using namespace ci;
using namespace std;

//----------------------------------------------------------------------------------------------------------------------

class PositionAndAngle
{
public:
  PositionAndAngle ()
  : m_position (0.f, 0.f), 
    m_angle (0.f)
  {
  }

  PositionAndAngle (float x, float y, float angle)
  : m_position (x,y), 
    m_angle (angle)
  {
  }

  PositionAndAngle operator * (const float rhs) const 
  {
    return PositionAndAngle (m_position.x * rhs, 
                             m_position.y * rhs, 
                             m_angle      * rhs);
  }

  PositionAndAngle operator + (const PositionAndAngle& rhs) const 
  {
    return PositionAndAngle (m_position.x + rhs.m_position.x, 
                             m_position.y + rhs.m_position.y, 
                             m_angle      + rhs.m_angle);
  }

public:
  float m_angle;
  Vec2f m_position;
};
