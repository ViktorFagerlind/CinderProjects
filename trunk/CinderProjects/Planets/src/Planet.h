#pragma once

#include "cinder/gl/gl.h"
#include "cinder/Vector.h"
#include "cinder/TriMesh.h"

#include "PhysicsObject.h"
#include "BasicObject.h"

using namespace ci;
using namespace std;

class Planet : public PhysicsObject, public BasicObject
{
public:
  Planet (const float           a, 
          const float           e, 
          const float           radius, 
          const float           initialVelocity, 
          const Vec3f&          color, 
          const PhysicsObject&  gravityCenter, 
          const string&         textureFile);

  void update();
  void draw();
  bool isDead () {return false;}

  float getRadius(){return mRadius;}

protected:
  const PhysicsObject& mGravityCenter;
  float mL;                 // angular momentum

  float mA;                 // major radius
  float mE;                 // excentricity
  float mTheta;             // position angle
  float mAxisAngle;			// rotation angle around local z-axis

  Vec3f mColor;

  gl::Texture   *mTexture;

  TriMesh       mMesh;
};
