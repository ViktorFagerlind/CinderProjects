#include "cinder/gl/gl.h"
#include "DynamicObject.h"
#include "math.h"

using namespace ci;
using namespace std;

DynamicObject::DynamicObject(float mass, Matrix44f inertia, Vec3f& cog)
  : PhysicsObject(mass, cog)
{
  mInertia  = inertia;
  mForceSum   = Vec3f(0.0f, 0.0f, 0.0f);
  mTorqueSum  = Vec3f(0.0f, 0.0f, 0.0f);
};

void DynamicObject::update()
{
  mAcceleration       = mForceSum/mMass;
  mRotationSpeedVect  = mTorqueSum/mMass;
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