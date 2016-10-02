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
  static bool isPointWithinCube (const vec3& point, const vec3& cubeCorner, const vec3& cubeSize)
  {
    return (point.x > cubeCorner.x && point.x < cubeCorner.x + cubeSize.x) &&
           (point.y > cubeCorner.y && point.y < cubeCorner.y + cubeSize.y) &&
           (point.z > cubeCorner.z && point.z < cubeCorner.z + cubeSize.z);
  }
};

