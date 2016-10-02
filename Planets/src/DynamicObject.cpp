#include "DynamicObject.h"
#include "GravityField.h"
#include "GameWorld.h"


DynamicObject::DynamicObject (const float mass, const vec3& position, float radius)
  : PhysicsObject(mass, position, radius)
{
  mVelocity         = vec3(0.0f, 0.0f, 0.0f);
  mAcceleration     = vec3(0.0f, 0.0f, 0.0f);
  mForceSum         = vec3(0.0f, 0.0f, 0.0f);
  mRotationVelocity = vec3(0.0f, 0.0f, 0.0f);
  mRotation         = vec3(0.0f, 0.0f, 0.0f);

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
  mForceSum     = vec3(0, 0, 0);

  mRotation     += mRotationVelocity;
}



