#include "cinder/gl/gl.h"
#include "DynamicObject.h"
#include "math.h"

using namespace ci;
using namespace std;

//Dynamic object using bounding box
DynamicObject::DynamicObject(float mass, Vec3f& cog,  float boundingBoxWidth, float boundingBoxHeight, float boundingBoxLength)
  : PhysicsObject(mass, cog, boundingBoxWidth, boundingBoxHeight, boundingBoxLength)
{
  DynamicObject::init(cog);
};

//Dynamic object using bounding plane
DynamicObject::DynamicObject(float mass, Vec3f& cog,  float boundingPlaneWidth, float boundingPlaneHeight)
  : PhysicsObject(mass, cog, boundingPlaneWidth, boundingPlaneHeight)
{
  DynamicObject::init(cog);
};

//Dynamic object using bounding sphere
DynamicObject::DynamicObject(float mass, Vec3f& cog,  float sphereRadius)
  : PhysicsObject(mass, cog, sphereRadius)
{
  DynamicObject::init(cog);
};


void DynamicObject::init(Vec3f& cog)
{
  mState.mLinearMomentum = Vec3f(0, 0, 0);

  Matrix44f globalInertia = mState.mOrientation*mLocalInertia*mState.mOrientation.transposed();
  Vec4f tempRot = Vec4f(0, 0, 0, 0);
  Vec4f mAngularMomentum = globalInertia*tempRot;
  mForceSum   = Vec3f(0.0f, 0.0f, 0.0f);
  mTorqueSum  = Vec3f(0.0f, 0.0f, 0.0f);
};



void DynamicObject::update(float dt)
{
  mState.mLinearMomentum  = EulerForward(mState.mLinearMomentum, mForceSum, dt);
  mState.mAngularMomentum = EulerForward(mState.mAngularMomentum, mTorqueSum, dt);

  PhysicsObject::update(dt);
}

void DynamicObject::draw()
{
  PhysicsObject::draw();

  gl::color(0.3f, 0.3f, 1.0f);
  gl::drawVector(mState.mPosition, mState.mPosition + getVelocity(), 10.0f, 5.0f);

  gl::color(0.3f, 1.0f, 0.3f);
  gl::drawVector(mState.mPosition + getVelocity(), mState.mPosition + getVelocity() + mForceSum/mMass, 10.0f, 5.0f);

  gl::color(1.0f, 0.3f, 0.3f);
  gl::drawVector(mState.mPosition, mState.mPosition + (50.0f * getRotationVelocity()), 10.0f, 5.0f);
}


void DynamicObject::applyForce(Vec3f force)
{
  mForceSum += force;
}

void DynamicObject::resetForce()
{
  mForceSum = Vec3f(0, 0, 0);
}

void DynamicObject::applyTorque(Vec3f pointOfAttack, Vec3f force)
{
  Vec3f lever = pointOfAttack - mState.mPosition;
  mTorqueSum += cross(lever, force);
}

void DynamicObject::resetTorque()
{
  mTorqueSum = Vec3f(0, 0, 0);
}