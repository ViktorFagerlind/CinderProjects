#pragma once

#include "Modifier.h"

#include "cinder/Vector.h"

using namespace ci;

class PointGravityModifier : public Modifier 
{
public:
	PointGravityModifier (const vec3& position, const float strength, const float maxStrength, const float radius)
  : mPosition(position),
    mStrength(strength),
    mMaxStrength(maxStrength),
    mRadiusSquared(radius*radius)
	{
	}
	
  inline void apply(Particle *const particle) 
	{
    const vec3 distance = mPosition - particle->mPosition;
    const float distanceSquared = distance.lengthSquared();

    if (distanceSquared > mRadiusSquared)
      return;

    float forceStrength = mStrength / distanceSquared;
    forceStrength = forceStrength > mMaxStrength ? mMaxStrength : forceStrength;

    particle->applyForce (distance.normalized() * forceStrength);
  }

  void setPosition (const vec3& position) 
	{
    mPosition = position;
  }

private:
  vec3         mPosition;
  const float   mStrength;
  const float   mRadiusSquared;
  const float   mMaxStrength;
};