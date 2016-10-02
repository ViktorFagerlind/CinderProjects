#pragma once

#include "cinder/app/App.h"
#include "Modifier.h"

#include "cinder/Vector.h"
#include "cinder/Perlin.h"

using namespace ci;

class PerlinModifier : public Modifier 
{
public:
	PerlinModifier (const float animationSpeed, const float forceStrength, const float dampStrength, const float scale = 0.0005f)
  : mAnimationCounter (0.0f),
    mAnimationSpeed   (animationSpeed),
    mForceStrength    (forceStrength),
    mDampStrength     (dampStrength),
    mScale            (scale)
	{
  	mPerlin.setSeed (clock());
	}
	
  void updateModifier () 
  {
	  mAnimationCounter += mAnimationSpeed;
  }

  inline void apply(Particle *const particle)
	{
    vec3 partlicePosition = particle->getPosition();
    vec3 particleVelocity = particle->getVelocity();

    vec3 force = mPerlin.dfBm (vec3 (partlicePosition.x, partlicePosition.y, mAnimationCounter) * mScale);

    force.z = 0;

    normalize(force);
    force = force * mForceStrength;

    particle->applyForce (force);  

    // apply damping
    force -= particleVelocity * length (particleVelocity) * mDampStrength;

    particle->applyForce(force);
  }

private:
	Perlin			mPerlin;
	float       mAnimationCounter;
  float       mForceStrength;
  float       mDampStrength;
  float       mAnimationSpeed;
  float       mScale;
};