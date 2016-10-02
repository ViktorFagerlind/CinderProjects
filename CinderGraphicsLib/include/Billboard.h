#include "cinder/Color.h"
#include "cinder/Vector.h"

using namespace ci;

class Billboard
{
public:
  static void drawBillboardTex (const vec3&   pos,
    const vec2&   scale,
    const float   *texCoords,
    const ColorAf& color = ColorAf::white (),
    const float    rotationDegrees = 0.f,
    const vec3&   bbRight = vec3 (1, 0, 0),
    const vec3&   bbUp = vec3 (0, 1, 0));

  static void drawBillboard (const vec3&   pos,
    const vec2&   scale,
    const ColorAf& color = ColorAf::white (),
    const float    rotationDegrees = 0.f,
    const vec3&   bbRight = vec3 (1, 0, 0),
    const vec3&   bbUp = vec3 (0, 1, 0));
};