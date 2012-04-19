#include "cinder/gl/gl.h"
#include "DynamicObject.h"
#include "math.h"

using namespace ci;
using namespace std;

//Dynamic object using bounding box
DynamicObject::DynamicObject(float mass, Vec3f& cog,  float boundingBoxWidth, float boundingBoxHeight, float boundingBoxLength)
  : PhysicsObject(mass, cog, boundingBoxWidth, boundingBoxHeight, boundingBoxLength)
{
  DynamicObject::init(mass, cog);
};

//Dynamic object using bounding plane
DynamicObject::DynamicObject(float mass, Vec3f& cog,  float boundingPlaneWidth, float boundingPlaneHeight)
  : PhysicsObject(mass, cog, boundingPlaneWidth, boundingPlaneHeight)
{
  DynamicObject::init(mass, cog);
};

//Dynamic object using bounding sphere
DynamicObject::DynamicObject(float mass, Vec3f& cog,  float sphereRadius)
  : PhysicsObject(mass, cog, sphereRadius)
{
  DynamicObject::init(mass, cog);
};


void DynamicObject::init(float mass, Vec3f& cog)
{
  // Inertia definition for a cube.
  float tempSideLength = 70.0f;
  mLocalInertia = Matrix44f::identity();
  mLocalInertia.m00 = (mMass / 6.0f)*tempSideLength*tempSideLength;
  mLocalInertia.m11 = (mMass / 6.0f)*tempSideLength*tempSideLength;  
  mLocalInertia.m22 = (mMass / 6.0f)*tempSideLength*tempSideLength; 
   
  mLocalInertiaInverted = mLocalInertia.inverted();
  mInertiaInverted = mOrientation*mLocalInertiaInverted*mOrientation.transposed();

  Vec4f tempRot = mRotationSpeedVect; 
  Vec4f mAngularMomentum = mInertia*tempRot;
  mForceSum   = Vec3f(0.0f, 0.0f, 0.0f);
  mTorqueSum  = Vec3f(0.0f, 0.0f, 0.0f);
};



void DynamicObject::update()
{
  float dt = 0.0167f;
  mAcceleration   = mForceSum/mMass;
  
  mAngularMomentum = EulerForward(mAngularMomentum, mTorqueSum, dt);
  mInertiaInverted = mOrientation*mLocalInertiaInverted*mOrientation.transposed();
  
  Vec4f tempRotationSpeedVect = mInertiaInverted * mAngularMomentum;
  mRotationSpeedVect.x = tempRotationSpeedVect.x;
  mRotationSpeedVect.y = tempRotationSpeedVect.y;
  mRotationSpeedVect.z = tempRotationSpeedVect.z;

  PhysicsObject::update();
  resetForce();
  resetTorque();
}

void DynamicObject::draw()
{
  PhysicsObject::draw();

  gl::color(0.3f, 0.3f, 1.0f);
  gl::drawVector(mPosition, mPosition + mVelocity, 10.0f, 5.0f);

  gl::color(0.3f, 1.0f, 0.3f);
  gl::drawVector(mPosition + mVelocity, mPosition + mVelocity + mAcceleration, 10.0f, 5.0f);

  gl::color(1.0f, 0.3f, 0.3f);
  gl::drawVector(mPosition, mPosition + (50.0f * mRotationSpeedVect), 10.0f, 5.0f);
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
  mTorqueSum += cross(pointOfAttack, force);
}

void DynamicObject::resetTorque()
{
  mTorqueSum = Vec3f(0, 0, 0);
}