#pragma once

#include "cinder/Vector.h"

using namespace ci;

class CollisionManager
{
public:
};

class Collisions
{
public:
  static bool isPointWithinCube (const Vec3f& point, const Vec3f& cubeCorner, const Vec3f& cubeSize)
  {
    return (point.x > cubeCorner.x && point.x < cubeCorner.x + cubeSize.x) &&
           (point.y > cubeCorner.y && point.y < cubeCorner.y + cubeSize.y) &&
           (point.z > cubeCorner.z && point.z < cubeCorner.z + cubeSize.z);
  }
};

