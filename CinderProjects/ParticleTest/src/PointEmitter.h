#include "Emitter.h"

#include "cinder/Rand.h"

using namespace ci;

class PointEmitter : public Emitter 
{
public:
	PointEmitter(const size_t maxNofParticles,
               Vec3f  position, 
							 std::string	particleImageFile, 
  						 float  particlesPerFrame, 
							 float 	minParticleSize,
							 float 	maxParticleSize,
							 float 	minParticleVelocity,
							 float 	maxParticleVelocity) 
  : Emitter (maxNofParticles, particleImageFile, particlesPerFrame),
    mPosition           (position),
	  mMinParticleVelocity(minParticleVelocity),
	  mMaxParticleVelocity(maxParticleVelocity), 
	  mMinParticleSize    (minParticleSize),
	  mMaxParticleSize    (maxParticleSize)

	{
  }

  void defineParticle (Particle* particle)
	{
		float particleSize = Rand::randFloat(mMinParticleSize, mMaxParticleSize);

	  particle->define(mPosition, particleSize, mMinParticleVelocity, mMaxParticleVelocity);
  }

private:
  Vec3f   mPosition;
	float 	mMinParticleVelocity;
	float 	mMaxParticleVelocity; 
	float 	mMinParticleSize;
	float 	mMaxParticleSize; 

};
