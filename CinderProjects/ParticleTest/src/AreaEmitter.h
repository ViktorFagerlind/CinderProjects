#include "Emitter.h"

#include "cinder/Rand.h"

using namespace ci;

class AreaEmitter : public Emitter 
{
public:
	AreaEmitter (const size_t maxNofParticles,
               Vec3f  position, 
							 std::string	particleImageFile, 
               const float particlesPerFrame,
  						 const float  width, 
							 const float 	height,
							 const float 	minParticleSize,
							 const float 	maxParticleSize,
							 const float 	minParticleVelocity,
							 const float 	maxParticleVelocity) 
  : Emitter (maxNofParticles, particleImageFile, particlesPerFrame),
    mPosition           (position),
    mWidth              (width),
    mHeight             (height),
	  mMinParticleVelocity(minParticleVelocity),
	  mMaxParticleVelocity(maxParticleVelocity), 
	  mMinParticleSize    (minParticleSize),
	  mMaxParticleSize    (maxParticleSize)
	{
  }

  void defineParticle (Particle* particle) 
	{
		float particleSize = Rand::randFloat(mMinParticleSize, mMaxParticleSize);

    Vec3f particlePosition = mPosition + Vec3f(Rand::randFloat(mWidth), Rand::randFloat(mHeight), 0);

	  particle->define(particlePosition, particleSize, mMinParticleVelocity, mMaxParticleVelocity);
  }

private:
  Vec3f   mPosition;
	float 	mMinParticleVelocity;
	float 	mMaxParticleVelocity; 
	float 	mMinParticleSize;
	float 	mMaxParticleSize; 
  float   mWidth; 
	float 	mHeight;
};
