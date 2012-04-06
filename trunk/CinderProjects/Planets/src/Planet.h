#pragma once

#include "cinder/gl/gl.h"
#include "cinder/Vector.h"
#include "cinder/gl/Vbo.h"
#include "cinder/gl/Texture.h"

#include <vector>

#include "PhysicsObject.h"
#include "BasicObject.h"
#include "BaseMaterial.h"

using namespace ci;
using namespace std;

class Planet : public PhysicsObject, public BasicObject
{
public:
  Planet (const float           a, 
          const float           e, 
          const float           radius, 
          const float           initialVelocity, 
          const PhysicsObject*  gravityCenter,
                shared_ptr<BaseMaterial>   material,
                gl::VboMesh     vbo);

  void update();
  void draw();
  bool isDead () {return false;}

  float getRadius(){return mRadius;}

protected:
  const PhysicsObject* mGravityCenter;
  float mL;                 // angular momentum

  float mA;                 // major radius
  float mE;                 // excentricity
  float mTheta;             // position angle
  float mAxisAngle;			    // rotation angle around local z-axis

  gl::VboMesh  mVbo;

  shared_ptr<BaseMaterial> mMaterial;
};
