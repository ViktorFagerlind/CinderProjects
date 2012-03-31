#pragma once

#include "Emitter.h"
#include "cinder/Rand.h"

#include "Math.h"

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
               const float  minRandVelocity,
							 const float  maxRandVelocity) 
  : Emitter (maxNofParticles, particlesPerFrame),
    mPosition           (position),
	  mBaseVelocity       (baseVelocity),
	  mMinRandVelocity    (minRandVelocity), 
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

    float randomNormedZ = Rand::randFloat (-1, 1);
    float xyPlaneAngle = asin (randomNormedZ); 
    float zAxisAngle = Rand::randFloat (0, 2.0f * (float)M_PI);

    float randomVelocity = Rand::randFloat (mMaxRandVelocity, mMaxRandVelocity);

    Vec3f randVelocity (randomVelocity * cos (xyPlaneAngle) * cos (zAxisAngle),
                        randomVelocity * cos (xyPlaneAngle) * sin (zAxisAngle),
                        randomVelocity * randomNormedZ);

    Vec3f particleVelocity = mBaseVelocity + randVelocity;

	  particle->define(mPosition, particleSize, particleVelocity);
  }

private:
  Vec3f   mPosition;
	Vec3f   mBaseVelocity;
	float   mMinRandVelocity; 
	float   mMaxRandVelocity; 
	float 	mMinParticleSize;
	float 	mMaxParticleSize; 

};
