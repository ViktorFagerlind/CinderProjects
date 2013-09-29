#include "Particle.h"

#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

// --------------------------------------------------------------------------------------------------------------------

Particle::Particle()
: mIsDead(true)
{
}

// --------------------------------------------------------------------------------------------------------------------

void Particle::define(const Vec3f& position, float size, Vec3f velocity)
{
  mLife         = Particle_fullLife_C;
  mOriginalSize = size;
  mCurrentSize  = size;
  mAcceleration = Vec3f::zero();
  mPosition     = position;
  mColor        = ColorAf(1, 1, 1, 1);
  mIsDead       = false;

  mVelocity = velocity;
}

// --------------------------------------------------------------------------------------------------------------------

void Particle::applyForce(const Vec3f& force) 
{
  mAcceleration += force;
}  

// --------------------------------------------------------------------------------------------------------------------

