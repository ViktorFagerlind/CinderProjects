#include "DynamicObject.h"
#include "GravityField.h"
#include "GameWorld.h"


DynamicObject::DynamicObject (const float mass, const Vec3f& position, float radius)
  : PhysicsObject(mass, position, radius)
{
  mVelocity         = Vec3f(0.0f, 0.0f, 0.0f);
  mAcceleration     = Vec3f(0.0f, 0.0f, 0.0f);
  mForceSum         = Vec3f(0.0f, 0.0f, 0.0f);
  mRotationVelocity = Vec3f(0.0f, 0.0f, 0.0f);
  mRotation         = Vec3f(0.0f, 0.0f, 0.0f);

  GameWorld::getGravityField()->addDynamicObject (this);
}

DynamicObject::~DynamicObject ()
{
  GameWorld::getGravityField ()->removeDynamicObject (this);
}

void DynamicObject::move()
{
  mAcceleration = mForceSum / mMass;
  mVelocity     += mAcceleration;
  mPosition     += mVelocity;
  mForceSum     = Vec3f(0, 0, 0);

  mRotation     += mRotationVelocity;
}



