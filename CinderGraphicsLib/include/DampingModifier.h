#include "Modifier.h"

#include "cinder/Color.h"

using namespace ci;

class DampingModifier : public Modifier 
{
public:
	DampingModifier (const float damping)
	{
    m_damping = damping;
	}
	
  void apply(Particle *const particle) 
	{
    const Vec3f velocity = particle->getVelocity();

    particle->applyForce (-velocity * velocity.length() * m_damping);
  }

private:

  float m_damping;
};