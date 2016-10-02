#pragma once

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
    const vec3 velocity = particle->getVelocity();

    particle->applyForce (-velocity * glm::length (velocity) * m_damping);
  }

private:

  float m_damping;
};