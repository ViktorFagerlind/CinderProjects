#pragma once
#include "cinder/Vector.h"
#include "cinder/Matrix.h"
#include "BoundingGeometry.h"

using namespace ci;
using namespace std;

class PhysicsObject
{
private:
  void init(float mass, Vec3f& cog);

public:
  PhysicsObject(float mass, Vec3f& cog, float boundingBoxWidth, float boundingBoxHeight, float boundingBoxLength);
  PhysicsObject(float mass, Vec3f& cog, float boundingPlaneWidth, float boundingPlaneHeight, Vec3f vertex1, Vec3f vertex2, Vec3f vertex3); //Init plane using 3 vertecies
  PhysicsObject(float mass, Vec3f& cog, float boundingPlaneWidth, float boundingPlaneHeight); //Init plane using position of centre of plane and orientation matrix
  PhysicsObject(float mass, Vec3f& cog, float boundingSphereRadius);
  ~PhysicsObject();
	void update (float dt);
	void draw();
  virtual void applyForce(Vec3f force){}
  virtual void resetForce(){}
  virtual void applyTorque(Vec3f pointOfAttack, Vec3f force){}
  virtual void resetTorque(){}

  void setPosition(Vec3f position);
  void setVelocity(Vec3f velocity);
  void setRotationVect(Vec3f rotationVect);
  float     EulerForward(float f, float fPrim, float dt);
  Vec3f     EulerForward(Vec3f f, Vec3f fPrim, float dt);
  Vec4f     EulerForward(Vec4f f, Vec4f fPrim, float dt);
  Matrix44f EulerForward(Matrix44f f, Matrix44f fPrim, float dt);
  void Orthogonalize(Matrix44f& matrix);
  const Matrix44f& getOrientation();

  Vec3f getPosition();
  Matrix44f getSkewMatrix(Vec4f vector);

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

  BoundingGeometry *mBoundingGeometry;
};
