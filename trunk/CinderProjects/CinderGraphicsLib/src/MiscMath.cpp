#pragma once

#include "MiscMath.h"

void MiscMath::ComputeTangent(const Vec3f& P1,  const Vec3f& P2,  const Vec3f& P3, 
                              const Vec2f& UV1, const Vec2f& UV2, const Vec2f& UV3,
                              Vec3f &tangent)
{
   Vec3f Edge1 = P2 - P1;
   Vec3f Edge2 = P3 - P1;
   Vec2f Edge1uv = UV2 - UV1;
   Vec2f Edge2uv = UV3 - UV1;

   float cp = Edge1uv.y * Edge2uv.x - Edge1uv.x * Edge2uv.y;

   if ( cp != 0.0f ) 
   {
      float mul = 1.0f / cp;
      tangent   = (Edge1 * -Edge2uv.y + Edge2 * Edge1uv.y) * mul;

      tangent.normalize ();
   }
}