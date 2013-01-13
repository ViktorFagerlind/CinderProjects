#pragma once

#include "cinder/Vector.h"
#include "cinder/Rand.h"

using namespace ci;
using namespace std;

class MiscMath
{
public:
  static void ComputeTangent(const Vec3f& P1,  const Vec3f& P2,  const Vec3f& P3, 
                             const Vec2f& UV1, const Vec2f& UV2, const Vec2f& UV3,
                             Vec3f &tangent);

  static Vec3f getRandomDirection ();

  // Projects v onto a plane with normal n
  // n must be of unity length
  static Vec3f projectOnPlane (const Vec3f& n, const Vec3f& v);

};
