#include "Particle.h"

#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

// --------------------------------------------------------------------------------------------------------------------

Particle::Particle()
: mIsDead(true)
{
}

// --------------------------------------------------------------------------------------------------------------------

Particle::~Particle ()
{
}

// --------------------------------------------------------------------------------------------------------------------

void Particle::define(const vec3& position, float size, vec3 velocity)
{
  mLife         = Particle_fullLife_C;
  mOriginalSize = size;
  mCurrentSize  = size;
  mAcceleration = vec3::zero();
  mPosition     = position;
  mColor        = ColorAf(1, 1, 1, 1);
  mIsDead       = false;

  mVelocity = velocity;
}

// --------------------------------------------------------------------------------------------------------------------

void Particle::applyForce(const vec3& force) 
{
  mAcceleration += force;
}  

// --------------------------------------------------------------------------------------------------------------------

void Particle::update ()
{
  mVelocity += mAcceleration;

  mPosition += mVelocity;

  mAcceleration = vec3::zero (); // Kill the acceleration for each update

  mIsDead = mLife <= 0.0;
}
