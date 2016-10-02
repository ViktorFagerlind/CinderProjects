#include "Emitter.h"

#include "cinder/Rand.h"
#include "BillboardParticleDrawer.h"

using namespace ci;

class AreaEmitter : public Emitter 
{
public:
	AreaEmitter (const size_t maxNofParticles,
               vec3  position, 
               const float particlesPerFrame,
  						 const float  width, 
							 const float 	height,
							 const float 	depth,
							 const float 	minParticleSize,
							 const float 	maxParticleSize,
							 const vec3& baseVelocity,
               const float  maxRandVelocity,
               shared_ptr<ParticleDrawerInterface> drawer = shared_ptr<ParticleDrawerInterface> (new BillboardParticleDrawer ()))
               : Emitter (maxNofParticles, particlesPerFrame, position, baseVelocity, 0.f, maxRandVelocity, drawer),
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

    vec3 particlePosition = mPosition + vec3(Rand::randFloat(-mWidth  / 2.0f, mWidth  / 2.0f), 
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
