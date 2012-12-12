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
  : Emitter (maxNofParticles, particlesPerFrame),
    mPosition           (position),
	  mBaseVelocity       (baseVelocity),
	  mMaxRandVelocity    (maxRandVelocity), 
	  mMinParticleSize    (minParticleSize),
	  mMaxParticleSize    (maxParticleSize)
	{
  }

  // - Randomly pick z
  // - Randomly pick angle around z-axis
  void defineParticle (Particle* particle)
	{
		float particleSize = Rand::randFloat(mMinParticleSize, mMaxParticleSize);
    Vec3f particleVelocity = mBaseVelocity + Emitter::getRandomVelocity (mMaxRandVelocity);

	  particle->define(mPosition, particleSize, particleVelocity);
  }

private:
  Vec3f   mPosition;
	Vec3f   mBaseVelocity;
	float   mMaxRandVelocity; 
	float 	mMinParticleSize;
	float 	mMaxParticleSize; 
};
