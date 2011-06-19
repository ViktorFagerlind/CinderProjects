#include "Modifier.h"

#include "cinder/Vector.h"

using namespace ci;

class GravityModifier : public Modifier 
{
public:
	GravityModifier (const Vec3f& gravity)
  : mGravity(gravity)
	{
	}
	
  void apply(Particle *const particle) 
	{
    particle->applyForce (mGravity);  
  }

private:
	const Vec3f mGravity;
};