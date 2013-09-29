#include "Emitter.h"

#include "cinder/Rand.h"

using namespace ci;

class AreaEmitter : public Emitter 
{
public:
	AreaEmitter (const size_t maxNofParticles,
               Vec3f  position, 
               const float particlesPerFrame,
  						 const float  width, 
							 const float 	height,
							 const float 	depth,
							 const float 	minParticleSize,
							 const float 	maxParticleSize,
							 const Vec3f& baseVelocity,
							 const float  maxRandVelocity) 
  : Emitter (maxNofParticles, particlesPerFrame, position, baseVelocity, 0.f, maxRandVelocity),
    mWidth              (width),
    mHeight             (height),
    mDepth              (depth),
	  mMinParticleSize    (minParticleSize),
	  mMaxParticleSize    (maxParticleSize)
	{
  }

  void defineParticle (Particle* particle) 
	{
		float particleSize = Rand::randFloat(mMinParticleSize, mMaxParticleSize);

    Vec3f particlePosition = mPosition + Vec3f(Rand::randFloat(-mWidth  / 2.0f, mWidth  / 2.0f), 
                                               Rand::randFloat(-mHeight / 2.0f, mHeight / 2.0f), 
                                               Rand::randFloat(-mDepth  / 2.0f, mDepth  / 2.0f));

	  particle->define(particlePosition, particleSize, getParticleVelocity ());
  }


private:
	float 	mMinParticleSize;
	float 	mMaxParticleSize; 
  float   mWidth; 
	float 	mHeight;
	float 	mDepth;
};
