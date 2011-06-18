#include "Modifier.h"

class CommonModifier : public Modifier 
{
public:
	CommonModifier (float lifeChange, 
	 							  float relativeStartSize, 
									float relativeEndSize, 
									float startOpacity,
									float endOpacity)
	{
    mLifeChange = lifeChange;

		mSizeM = relativeEndSize;
		mSizeK = (relativeStartSize - relativeEndSize) / Particle_fullLife_C;
		
		mOpacityM = endOpacity;
		mOpacityK = (startOpacity - endOpacity) / Particle_fullLife_C;
	}
	
  void apply(Particle *const particle) 
	{
    particle->setLife(particle->getLife() - mLifeChange);
    
    particle->scaleSize(particle->getLife() * mSizeK + mSizeM);
    
    particle->setOpacity(255.0f * (particle->getLife() * mOpacityK + mOpacityM));
  }

private:
	float mLifeChange;
	float mSizeK;
	float mSizeM;
	float mOpacityK;
	float mOpacityM;
};