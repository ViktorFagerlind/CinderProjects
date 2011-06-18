#include "Emitter.h"

#include "cinder/Rand.h"

using namespace ci;
using namespace std;

class PointEmitter : public Emitter 
{
public:
	PointEmitter(Vec3f  position, 
							 string	particleImageFile, 
  						 float  particlesPerFrame, 
							 float 	minParticleSize,
							 float 	maxParticleSize,
							 float 	minParticleVelocity,
							 float 	maxParticleVelocity) 
  : Emitter (particleImageFile, particlesPerFrame),
    mPosition           (position),
	  mMinParticleVelocity(minParticleVelocity),
	  mMaxParticleVelocity(maxParticleVelocity), 
	  mMinParticleSize    (minParticleSize),
	  mMaxParticleSize    (maxParticleSize)

	{
  }

  Particle* createParticle() 
	{
		float particleSize = Rand::randFloat(mMinParticleSize, mMaxParticleSize);

	  return new Particle(mPosition, particleSize, mMinParticleVelocity, mMaxParticleVelocity);
  }

private:
  Vec3f   mPosition;
	float 	mMinParticleVelocity;
	float 	mMaxParticleVelocity; 
	float 	mMinParticleSize;
	float 	mMaxParticleSize; 

};
