#pragma once

#include "cinder/Vector.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Vbo.h"

using namespace ci;
using namespace std;

class March
{
public:
  March ();

  void draw ();

private:
  void setupTetraVbo ();

  gl::VboMesh mVboMesh;

};