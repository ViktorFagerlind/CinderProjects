#pragma once

#include "cinder/Vector.h"
#include "cinder/gl/GlslProg.h"

using namespace ci;

class TubeDrawer
{
public:
  static void draw (gl::GlslProg&   shader, 
                    const Vec3f&    upDirection,
                    const bool      blendFirst,
                    const uint32_t  nofCircularSegments,
                          Vec3f    *points,
                          Vec3f    *normals,
                          float    *radie,
                    const uint32_t  nofPoints);

  static void debugDraw (const Vec3f&     upDirection,
                         Vec3f           *points,
                         Vec3f           *normals,
                         float           *radie,
                         const uint32_t   nofPoints);

};

