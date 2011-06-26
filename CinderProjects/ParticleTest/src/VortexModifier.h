#include "Modifier.h"

#include "cinder/Vector.h"

using namespace ci;

class VortexModifier : public Modifier 
{
public:
	VortexModifier (const Vec3f& position, const float strength, const float damping, const float radius, const float angle)
	{
    mDirection  = Vec3f(0,0,-1);
    mPosition   = position;
    mStrength   = strength;
    mDamping    = damping;
    mRadius     = radius;
    mAngle      = angle;
	}
	
  void apply(Particle *const particle) 
	{
    const Vec3f particlePosition    = particle->getPosition();
    Vec3f relativeParticlePosition  = particlePosition - mPosition;
    const Vec3f particleVelocity    = particle->getVelocity();
    float distance = relativeParticlePosition.length();
    Vec3f force;
    float forceStrength;

    if (distance > mRadius)
      return;
    
    forceStrength = mStrength * (mRadius - distance) / mRadius;

    force = mDirection.cross (relativeParticlePosition);

    force.rotateZ(  mAngle);
    
    force.normalize();
    force *= forceStrength;

    // apply damping
    force -= particleVelocity * particleVelocity.length() * mDamping;

    particle->applyForce(force);
  }

private:
  Vec3f mPosition;
  Vec3f mDirection;
  float mAngle;
  float mStrength;
  float mRadius;
  float mDamping;
};