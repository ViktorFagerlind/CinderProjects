#pragma once

#include "Modifier.h"

#include "cinder/Vector.h"

using namespace ci;

class GravityModifier : public Modifier 
{
public:
	GravityModifier (const vec3& gravity)
  : mGravity(gravity)
	{
	}
	
  inline void apply(Particle *const particle) 
	{
    particle->applyForce (mGravity);  
  }

private:
	const vec3 mGravity;
};