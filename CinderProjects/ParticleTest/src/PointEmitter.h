#pragma once

#include "Emitter.h"
#include "cinder/Rand.h"

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
							 const Vec3f& randVelocity) 
  : Emitter (maxNofParticles, particlesPerFrame),
    mPosition           (position),
	  mBaseVelocity       (baseVelocity),
	  mRandVelocity       (randVelocity), 
	  mMinParticleSize    (minParticleSize),
	  mMaxParticleSize    (maxParticleSize)
	{
  }

  void defineParticle (Particle* particle)
	{
		float particleSize = Rand::randFloat(mMinParticleSize, mMaxParticleSize);

    Vec3f particleVelocity = Vec3f (mBaseVelocity.x + Rand::randFloat (-mRandVelocity.x, mRandVelocity.x),
                                    mBaseVelocity.y + Rand::randFloat (-mRandVelocity.y, mRandVelocity.y),
                                    mBaseVelocity.z + Rand::randFloat (-mRandVelocity.z, mRandVelocity.z));

	  particle->define(mPosition, particleSize, particleVelocity);
  }

private:
  Vec3f   mPosition;
	Vec3f   mBaseVelocity;
	Vec3f   mRandVelocity; 
	float 	mMinParticleSize;
	float 	mMaxParticleSize; 

};
