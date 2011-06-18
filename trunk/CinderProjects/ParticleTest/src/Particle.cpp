#include "Particle.h"

#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

// --------------------------------------------------------------------------------------------------------------------

Particle::Particle(Vec3f position, float size, Vec3f velocity)
: mLife(Particle_fullLife_C),
  mOriginalSize(size),
  mCurrentSize(size),
  mAcceleration(Vec3f::zero()), 
  mVelocity(velocity), 
  mPosition(position),
  mOpacity(1)
{
}

// --------------------------------------------------------------------------------------------------------------------

Particle::Particle(Vec3f position, float size, float minVelocity, float maxVelocity)
: mLife(Particle_fullLife_C),
  mOriginalSize(size),
  mCurrentSize(size),
  mAcceleration(Vec3f::zero()), 
  mPosition(position),
  mOpacity(255.0)
{
  mVelocity = Vec3f(Rand::randFloat (minVelocity, maxVelocity),
                    Rand::randFloat (minVelocity, maxVelocity),
                    Rand::randFloat (minVelocity, maxVelocity));
}

// --------------------------------------------------------------------------------------------------------------------

void Particle::applyForce(Vec3f force) 
{
  mAcceleration += force;
}  

// --------------------------------------------------------------------------------------------------------------------

void Particle::update() 
{
  mVelocity += mAcceleration;
    
  mPosition += mVelocity;

  mAcceleration = Vec3f::zero(); // Kill the acceleration for each update
}

// --------------------------------------------------------------------------------------------------------------------

void Particle::draw() 
{
  glColor3f (1, 0 , 0);

	glVertex3f(mPosition.x-mCurrentSize, mPosition.y-mCurrentSize, mPosition.z);
	glVertex3f(mPosition.x+mCurrentSize, mPosition.y-mCurrentSize, mPosition.z);
	glVertex3f(mPosition.x+mCurrentSize, mPosition.y+mCurrentSize, mPosition.z);
	glVertex3f(mPosition.x-mCurrentSize, mPosition.y+mCurrentSize, mPosition.z);
}

// --------------------------------------------------------------------------------------------------------------------

bool Particle::dead() 
{
  return mLife <= 0.0;
}
