#pragma once

#include "cinder/Vector.h"
#include "cinder/Rand.h"

using namespace ci;
using namespace std;

class MiscMath
{
public:
  static void ComputeTangent(const vec3& P1,  const vec3& P2,  const vec3& P3, 
                             const vec2& UV1, const vec2& UV2, const vec2& UV3,
                             vec3 &tangent);

  static vec3 getRandomDirection ();

  // Projects v onto a plane with normal n
  // n must be of unity length
  static vec3 projectOnPlane (const vec3& n, const vec3& v);

};
