#pragma once

#include "cinder/Vector.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace std;

class Triangle
{
public:
  Vec3f p[3];
};

class March
{
public:
  static void draw ();

};