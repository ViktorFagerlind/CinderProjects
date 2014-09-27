#include "PhysicsObject.h"

PhysicsObject::PhysicsObject (const float mass, const Vec3f& position, float radius)
{
  mMass     = mass;
  mPosition = position;
  mRadius   = radius;      
}
