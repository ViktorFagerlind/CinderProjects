#include "cinder/app/AppBasic.h"
#include "Modifier.h"

#include "cinder/Vector.h"
#include "cinder/Perlin.h"

using namespace ci;

class PerlinModifier : public Modifier 
{
public:
	PerlinModifier (const float animationSpeed, const float forceStrength, const float dampStrength)
  : mAnimationCounter (0.0f),
    mAnimationSpeed   (animationSpeed),
    mForceStrength    (forceStrength),
    mDampStrength     (dampStrength)
	{
  	mPerlin.setSeed (clock());
	}
	
  void updateModifier () 
  {
	  mAnimationCounter += mAnimationSpeed;
  }

  inline void apply(Particle *const particle)
	{
    Vec3f partlicePosition = particle->getPosition();
    Vec3f particleVelocity = particle->getVelocity();

		Vec3f force = mPerlin.dfBm( Vec3f(partlicePosition.x, partlicePosition.y, mAnimationCounter) * 0.0005f );

    force.z = 0;

    force.normalize();
    force = force * mForceStrength;

    particle->applyForce (force);  

    // apply damping
    force -= particleVelocity * particleVelocity.length() * mDampStrength;

    particle->applyForce(force);
  }

private:
	Perlin			mPerlin;
	float       mAnimationCounter;
  float       mForceStrength;
  float       mDampStrength;
  float       mAnimationSpeed;
};