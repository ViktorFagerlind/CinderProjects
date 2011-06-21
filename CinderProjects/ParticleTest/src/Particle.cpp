#include "Particle.h"

#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

// --------------------------------------------------------------------------------------------------------------------

Particle::Particle()
: mIsDead(true)
{
}

// --------------------------------------------------------------------------------------------------------------------

void Particle::define(const Vec3f& position, float size, float minVelocity, float maxVelocity)
{
  mLife         = Particle_fullLife_C;
  mOriginalSize = size;
  mCurrentSize  = size;
  mAcceleration = Vec3f::zero();
  mPosition     = position;
  mColor        = ColorAf(1, 1, 1, 1);
  mIsDead       = false;

  mVelocity = Vec3f(Rand::randFloat (minVelocity, maxVelocity),
                    Rand::randFloat (minVelocity, maxVelocity),
                    Rand::randFloat (minVelocity, maxVelocity));
}

// --------------------------------------------------------------------------------------------------------------------

void Particle::applyForce(const Vec3f& force) 
{
  mAcceleration += force;
}  

// --------------------------------------------------------------------------------------------------------------------

