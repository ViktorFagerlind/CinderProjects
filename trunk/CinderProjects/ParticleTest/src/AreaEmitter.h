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
							 const float 	minParticleSize,
							 const float 	maxParticleSize,
							 const float 	minParticleVelocity,
							 const float 	maxParticleVelocity) 
  : Emitter (maxNofParticles, particlesPerFrame),
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

    Vec3f particlePosition = mPosition + Vec3f(Rand::randFloat(-mWidth/2.0f, mWidth/2.0f), Rand::randFloat(-mHeight/2.0f, mHeight/2.0f), 0);

	  particle->define(particlePosition, particleSize, mMinParticleVelocity, mMaxParticleVelocity);
  }

  void setPosition (const Vec3f& position)
  {
    mPosition = position;
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
