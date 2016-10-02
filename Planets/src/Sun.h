#pragma once

#include "cinder/gl/gl.h"
#include "cinder/gl/Vbo.h"
#include "cinder/gl/GlslProg.h"

#include "BasicObject.h"
#include "PhysicsObject.h"

class ParticleSystem;

using namespace ci;
using namespace std;

class Sun : public PhysicsObject, public BasicObject
{
public:
  Sun (const float                    mass, 
       const vec3&                   position, 
       const float                    radius,   
             gl::VboMesh              vbo);

  ~Sun ();

  void setLightPosition ();

  void update ();
  void draw ();
  bool isDead ()    {return false;}

  float getRadius() {return mRadius;}

protected:
  gl::VboMesh  mVbo;

  gl::GlslProg  mShader;

  // Keep track of own particle system to bloom it
  ParticleSystem *mSunParticleSystem;
};
