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
    mAnimationCounter = 0;
	}
	
  void update () 
  {
	  mAnimationCounter += 10.0f; // move ahead in time, which becomes the z-axis of our 3D noise
  }

  void apply(Particle *const particle) 
	{
    Vec3f partlicePosition = particle->getPosition();
    Vec3f particleVelocity = particle->getVelocity();

		Vec3f force = mPerlin.dfBm( Vec3f( partlicePosition.x, partlicePosition.y, mAnimationCounter) * 0.0005f );
//		partIt->mZ = deriv.z;
//		Vec2f deriv2( deriv.x, deriv.y );
//		deriv2.normalize();
//		partIt->mVelocity += deriv2 * SPEED;

    force.z = 0;

    force.normalize();
    force = force * 1.0f;

    particle->applyForce (force);  

    // apply damping
    force -= particleVelocity * particleVelocity.lengthSquared() * 0.001f;

    particle->applyForce(force);
  }

private:
	const Vec3f mGravity;
	Perlin			mPerlin;
	size_t      mAnimationCounter;
};