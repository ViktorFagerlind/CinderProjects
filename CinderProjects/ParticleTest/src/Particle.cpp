#include "Particle.h"

#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

static const float fullLife_C = 100.0;

// --------------------------------------------------------------------------------------------------------------------

Particle::Particle(Vec3f position, Vec3f velocity, float size)
: mLife(fullLife_C),
  mAcceleration(Vec3f::zero()), 
  mVelocity(velocity), 
  mPosition(position),
  mOriginalSize(size),
  mCurrentSize(size),
  mOpacity(1)
{
}

// --------------------------------------------------------------------------------------------------------------------

Particle::Particle(Vec3f position, float minVelocity, float maxVelocity, float size)
: mLife(fullLife_C),
  mAcceleration(Vec3f::zero()), 
  mPosition(position),
  mOriginalSize(size),
  mCurrentSize(size),
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
