#pragma once

#include "Modifier.h"

class CommonModifier : public Modifier 
{
public:
	CommonModifier (float lifeChange, 
	 							  float relativeStartSize, 
									float relativeEndSize)
	{
    mLifeChange = lifeChange;

		mSizeM = relativeEndSize;
		mSizeK = (relativeStartSize - relativeEndSize) / Particle_fullLife_C;
	}
	
  inline void apply(Particle *const particle) 
	{
    particle->decreaseLife (mLifeChange);
    
    particle->scaleSize(particle->getLife() * mSizeK + mSizeM);
  }

private:
	float mLifeChange;
	float mSizeK;
	float mSizeM;
};