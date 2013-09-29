#pragma once

#include "Emitter.h"

using namespace ci;

class PointEmitter : public Emitter 
{
public:
	PointEmitter(const size_t maxNofParticles,
               const Vec3f&  position, 
  						 const float  particlesPerFrame, 
							 const float 	minParticleSize,
							 const float 	maxParticleSize,
							 const Vec3f& baseVelocity,
							 const float  maxRandVelocity) 
  : Emitter (maxNofParticles, particlesPerFrame, position, baseVelocity, 0.f, maxRandVelocity),
	  mMinParticleSize    (minParticleSize),
	  mMaxParticleSize    (maxParticleSize)
	{
  }

  // - Randomly pick z
  // - Randomly pick angle around z-axis
  void defineParticle (Particle* particle)
	{
		float particleSize = Rand::randFloat(mMinParticleSize, mMaxParticleSize);

	  particle->define (mPosition, particleSize, getParticleVelocity ());
  }


private:
	float 	mMinParticleSize;
	float 	mMaxParticleSize; 
};
