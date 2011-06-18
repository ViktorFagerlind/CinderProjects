#include "Emitter.h"

Emitter::Emitter(std::string particleImageFile, float particlesPerFrame) 
{
  // particleImage = p.loadImage(particleImageFile);
    
  mParticlesPerFrameCount = 0;
  mParticlesPerFrame = particlesPerFrame;
  mKilled = false;
}

void Emitter::update()
{
  mParticlesPerFrameCount += mParticlesPerFrame;
  	
  if (!mKilled && mParticlesPerFrameCount >= 1.0f)
  {
	  // create and add particles
	  for (int i=(int)mParticlesPerFrameCount; i > 0; i--)
	  {
	    mParticles.push_back(createParticle());
	  }
	    
	  mParticlesPerFrameCount = 0.0f;
  }
  	
  // Update particles
  for (list<Particle*>::iterator pit = mParticles.begin(); pit != mParticles.end();)
  {
    Particle *p = *pit;

    // Update modifiers
    for (list<Modifier*>::iterator mit = mModifiers.begin(); mit != mModifiers.end(); mit++)
    {
      Modifier *m = *mit;
        
      m->apply(p);
    }
      
    p->update();
      
    if (p->dead())
    {
      pit = mParticles.erase(pit);
    }
    else
      pit++;
  }
    
}

void Emitter::draw()
{
  glBegin(GL_QUADS);

  //p.texture(particleImage);
  // Draw all particles
  for (list<Particle*>::iterator it = mParticles.begin(); it != mParticles.end(); it++)
    (*it)->draw();
    
  glEnd();
}
