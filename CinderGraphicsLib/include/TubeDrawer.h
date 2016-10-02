#pragma once

#include "cinder/Vector.h"
#include "cinder/gl/GlslProg.h"

using namespace ci;

class TubeDrawer
{
public:
  static void draw (gl::GlslProg&   shader, 
                    const vec3&    upDirection,
                    const bool      blendFirst,
                    const uint32_t  nofCircularSegments,
                          vec3    *points,
                          vec3    *normals,
                          float    *radie,
                    const uint32_t  nofPoints);

  static void debugDraw (const vec3&     upDirection,
                         vec3           *points,
                         vec3           *normals,
                         float           *radie,
                         const uint32_t   nofPoints);

};

