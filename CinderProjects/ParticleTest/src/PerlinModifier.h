#include "cinder/app/AppBasic.h"
#include "Modifier.h"

#include "cinder/Vector.h"
#include "cinder/Perlin.h"

using namespace ci;

class PerlinModifier : public Modifier 
{
public:
	PerlinModifier ()
	{
  	mPerlin.setSeed (clock());
    mAnimationCounter = 0.0f;
	}
	
  void update () 
  {
	  mAnimationCounter += 10.0f; // move ahead in time, which becomes the z-axis of our 3D noise
  }

  inline void apply(Particle *const particle)
	{
    Vec3f partlicePosition = particle->getPosition();
    Vec3f particleVelocity = particle->getVelocity();

		Vec3f force = mPerlin.dfBm( Vec3f( partlicePosition.x, partlicePosition.y, mAnimationCounter) * 0.0005f );

    force.z = 0;

    force.normalize();
    force = force * 1.0f;

    particle->applyForce (force);  

    // apply damping
    force -= particleVelocity * particleVelocity.length() * 0.01f;

    particle->applyForce(force);
  }

private:
	const Vec3f mGravity;
	Perlin			mPerlin;
	float       mAnimationCounter;
};