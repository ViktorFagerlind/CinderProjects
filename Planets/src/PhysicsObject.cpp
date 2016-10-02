#include "PhysicsObject.h"

PhysicsObject::PhysicsObject (const float mass, const vec3& position, float radius)
{
  mMass     = mass;
  mPosition = position;
  mRadius   = radius;      
}
