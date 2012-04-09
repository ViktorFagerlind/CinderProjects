#pragma once
#include "cinder/Vector.h"
#include "cinder/Matrix.h"

using namespace ci;
using namespace std;

class PhysicsObject
{
  public:
  PhysicsObject(float mass, Vec3f& cog);
	void update();
	void draw();
  void setPosition(Vec3f position);
  void setVelocity(Vec3f velocity);
  void setRotationVect(Vec3f rotationVect);
  float     EulerForward(float f, float fPrim, float dt);
  Vec3f     EulerForward(Vec3f f, Vec3f fPrim, float dt);
  Matrix44f EulerForward(Matrix44f f, Matrix44f fPrim, float dt);
  void Orthogonalize(Matrix44f& matrix);
  Matrix44f getSkewMatrix(Vec3f vector);

  Vec3f getPosition();

protected:
  float     mMass;
  Vec3f     mCoG;
  
  Vec3f     mPosition;
  Vec3f     mVelocity;
  Vec3f     mAcceleration;
  Vec3f     mRotationVect;
  Vec3f     mRotationSpeedVect;
  Matrix44f mOrientation;
  Matrix44f mOrientationSpeed;
};
