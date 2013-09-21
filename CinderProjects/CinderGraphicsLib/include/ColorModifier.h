#include "Modifier.h"

#include "cinder/Color.h"

using namespace ci;

class ColorModifier : public Modifier 
{
public:
	ColorModifier (const ColorAf& startColor, const ColorAf& middleColor, const ColorAf& endColor, const float middleTime)
	{
    mMiddleTime = middleTime*Particle_fullLife_C;
    mColorM1 = startColor;
    mColorK1 = (middleColor - startColor) / mMiddleTime;

    mColorK2 = (endColor - middleColor) / (Particle_fullLife_C - mMiddleTime);
    mColorM2 = middleColor - mColorK2 * mMiddleTime;
	}
	
  void apply(Particle *const particle) 
	{
    const float time = Particle_fullLife_C - particle->getLife();

    if (time < mMiddleTime)
      particle->setColor (mColorK1 * time + mColorM1);  
    else
      particle->setColor (mColorK2 * time + mColorM2);  
  }

private:
  ColorAf mColorK1;
  ColorAf mColorM1;
  ColorAf mColorK2;
  ColorAf mColorM2;

  float mMiddleTime;
};