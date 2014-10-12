#include "cinder/Color.h"
#include "cinder/Vector.h"

using namespace ci;

class Billboard
{
public:
  static void drawBillboardTex (const Vec3f&   pos,
    const Vec2f&   scale,
    const float   *texCoords,
    const ColorAf& color = ColorAf::white (),
    const float    rotationDegrees = 0.f,
    const Vec3f&   bbRight = Vec3f (1, 0, 0),
    const Vec3f&   bbUp = Vec3f (0, 1, 0));

  static void drawBillboard (const Vec3f&   pos,
    const Vec2f&   scale,
    const ColorAf& color = ColorAf::white (),
    const float    rotationDegrees = 0.f,
    const Vec3f&   bbRight = Vec3f (1, 0, 0),
    const Vec3f&   bbUp = Vec3f (0, 1, 0));
};