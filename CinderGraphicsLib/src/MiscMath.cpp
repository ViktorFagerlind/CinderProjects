#pragma once

#include "MiscMath.h"

void MiscMath::ComputeTangent(const vec3& P1,  const vec3& P2,  const vec3& P3, 
                              const vec2& UV1, const vec2& UV2, const vec2& UV3,
                              vec3 &tangent)
{
   vec3 Edge1 = P2 - P1;
   vec3 Edge2 = P3 - P1;
   vec2 Edge1uv = UV2 - UV1;
   vec2 Edge2uv = UV3 - UV1;

   float cp = Edge1uv.y * Edge2uv.x - Edge1uv.x * Edge2uv.y;

   if ( cp != 0.0f ) 
   {
      float mul = 1.0f / cp;
      tangent   = (Edge1 * -Edge2uv.y + Edge2 * Edge1uv.y) * mul;

      tangent.normalize ();
   }
}

vec3 MiscMath::getRandomDirection ()
{
  float randomNormedZ = Rand::randFloat (-1, 1);
  float xyPlaneAngle = asin (randomNormedZ); 
  float zAxisAngle = Rand::randFloat (0, 2.0f * (float)M_PI);

  return vec3 (cos (xyPlaneAngle) * cos (zAxisAngle),
                cos (xyPlaneAngle) * sin (zAxisAngle),
                randomNormedZ);
}

vec3 MiscMath::projectOnPlane (const vec3& n, const vec3& v)
{
  return v - n * n.dot (v);
}