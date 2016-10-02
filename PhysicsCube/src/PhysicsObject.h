#pragma once
#include "cinder/Vector.h"
#include "cinder/Matrix.h"
#include "BoundingGeometry.h"

using namespace ci;
using namespace std;

class State
{
public:
  State();

  vec3     mPosition;
  //vec3     mVelocity;
  //vec3     mAcceleration;
  //vec3     mRotationVect;
  //vec3     mRotationSpeedVect;
  vec3     mLinearMomentum;
  Vec4f     mAngularMomentum;
  Matrix44f mOrientation;
  Matrix44f mOrientationSpeed;
};

class PhysicsObject
{
private:
  void init(float mass, vec3& cog);

public:
  PhysicsObject(float mass, vec3& cog, float boundingBoxWidth, float boundingBoxHeight, float boundingBoxLength);
  PhysicsObject(float mass, vec3& cog, float boundingPlaneWidth, float boundingPlaneHeight, vec3 vertex1, vec3 vertex2, vec3 vertex3); //Init plane using 3 vertecies
  PhysicsObject(float mass, vec3& cog, float boundingPlaneWidth, float boundingPlaneHeight); //Init plane using position of centre of plane and orientation matrix
  PhysicsObject(float mass, vec3& cog, float boundingSphereRadius);
  ~PhysicsObject();
	void update (float dt);
	void draw();
  virtual void applyForce(vec3 force){}
  virtual void resetForce(){}
  virtual void applyTorque(vec3 pointOfAttack, vec3 force){}
  virtual void resetTorque(){}

  void setPosition(vec3 position);
  void setVelocity(vec3 velocity);
  //void setRotationVect(vec3 rotationVect);
  //void setRotationSpeedVect(vec3 rotationSpeedVect);
  void setLinearMomentum(vec3 linearMomentum);
  void setAngularMomentum(Vec4f angularMomentum);
  float     EulerForward(float f, float fPrim, float dt);
  vec3     EulerForward(vec3 f, vec3 fPrim, float dt);
  Vec4f     EulerForward(Vec4f f, Vec4f fPrim, float dt);
  Matrix44f EulerForward(Matrix44f f, Matrix44f fPrim, float dt);
  void Orthogonalize(Matrix44f& matrix);


  vec3 getPosition();
  vec3 getVelocity();
  vec3 getRotationVelocity();

  vec3 getPointVelocity(vec3 point);
  const Matrix44f& getOrientation();
  Matrix44f getGlobalInertiaInverted();
  Matrix44f getSkewMatrix(Vec4f vector);

  BoundingGeometry *mBoundingGeometry;
  State     mState;
  float     mMass;
  vec3     mCoG;
  Matrix44f mLocalInertia;
  Matrix44f mLocalInertiaInverted;
  Matrix44f mInertia;
  Matrix44f mInertiaInverted;
};
