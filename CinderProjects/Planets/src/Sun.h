#pragma once

#include "cinder/gl/gl.h"
#include "cinder/Vector.h"
#include "cinder/gl/Vbo.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Texture.h"

#include <vector>

#include "PhysicsObject.h"
#include "BasicObject.h"
#include "BaseMaterial.h"

using namespace ci;
using namespace std;

class Sun : public PhysicsObject, public BasicObject
{
public:
  Sun (const float                    mass, 
       const Vec3f&                   position, 
       const float                    radius,   
             gl::VboMesh              vbo);

  void setLightPosition ();

  void update ();
  void draw ();
  bool isDead ()    {return false;}

  float getRadius() {return mRadius;}

protected:
  gl::VboMesh  mVbo;

  gl::GlslProg  mShader;
};
