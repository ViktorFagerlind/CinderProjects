#pragma once

#include "Modifier.h"

#include "cinder/Vector.h"

using namespace ci;

class VortexModifier : public Modifier 
{
public:
	VortexModifier (const vec3& position, const float strength, const float damping, const float radius, const float angle)
  : mDirection (vec3(0,0,-1)),
    mPosition (position),
    mStrength (strength),
    mDamping (damping),
    mRadius (radius),
    mAngle (angle)
	{
	}
	
  void apply(Particle *const particle) 
	{
    const vec3 particlePosition    = particle->getPosition();
    vec3 relativeParticlePosition  = particlePosition - mPosition;
    const vec3 particleVelocity    = particle->getVelocity();
    float distance = relativeParticlePosition.length();
    vec3 force;
    float forceStrength;

    if (distance > mRadius)
      return;
    
    forceStrength = mStrength * (mRadius - distance) / mRadius;

    force = mDirection.cross (relativeParticlePosition);

    force.rotateZ(  mAngle);
    
    force.normalize();
    force *= forceStrength;

    // apply damping
    force -= particleVelocity * particleVelocity.length() * mDamping;

    particle->applyForce(force);
  }

  void setPosition (const vec3& position) 
	{
    mPosition = position;
  }

private:
  vec3       mPosition;
  const vec3 mDirection;
  const float mAngle;
  const float mStrength;
  const float mRadius;
  const float mDamping;
};