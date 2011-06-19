#include "Emitter.h"

Emitter::Emitter(std::string particleImageFile, const float particlesPerFrame) 
{
  mParticleTexture = new gl::Texture (loadImage (loadFile (particleImageFile)));
    
  mParticlesPerFrameCount = 0;
  mParticlesPerFrame = particlesPerFrame;
  mKilled = false;
}

Emitter::~Emitter()
{
  for (list<Particle*>::iterator it = mParticles.begin(); it != mParticles.end(); it++)
  {
    delete (*it);
  }

  for (list<Modifier*>::iterator it = mModifiers.begin(); it != mModifiers.end(); it++)
  {
    delete (*it);
  }
  
  mParticles.clear();
  mModifiers.clear();
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
  	
  // Update modifiers
  for (list<Modifier*>::iterator mit = mModifiers.begin(); mit != mModifiers.end(); mit++)
  {
    Modifier *m = *mit;
    m->update();
  }

  // Update particles
  for (list<Particle*>::iterator pit = mParticles.begin(); pit != mParticles.end();)
  {
    Particle *p = *pit;

    // Update particles for modifiers
    for (list<Modifier*>::iterator mit = mModifiers.begin(); mit != mModifiers.end(); mit++)
    {
      Modifier *m = *mit;
        
      m->apply(p);
    }
      
    p->update();
      
    if (p->dead())
    {
      delete (p);
      pit = mParticles.erase(pit);
    }
    else
      pit++;
  }
    
}

void Emitter::draw()
{
	glEnable( GL_TEXTURE_2D );
  mParticleTexture->bind();

  glBegin(GL_QUADS);

  // Draw all particles
  for (list<Particle*>::iterator it = mParticles.begin(); it != mParticles.end(); it++)
    (*it)->draw();
    
  glEnd();
}
