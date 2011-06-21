#include "Emitter.h"

Emitter::Emitter(std::string particleImageFile, const float particlesPerFrame) 
: mMaxNofParticles(200000),
  mParticleCount(0)
{
  mParticleTexture = new gl::Texture (loadImage (loadFile (particleImageFile)));
    
  mParticlesToCreate = 0.0f;
  mParticlesPerFrame = particlesPerFrame;
  mKilled = false;

  mParticles = new Particle[mMaxNofParticles];
}

Emitter::~Emitter()
{
  delete [] mParticles;
}

void Emitter::update()
{
  // Update how many particles to create
  mParticlesToCreate += mParticlesPerFrame;

  // Update how many particles to create this frame
  size_t nofParticlesToCreateThisFrame = mKilled ? 0 : (size_t)mParticlesToCreate;

  // update particles left to create
  mParticlesToCreate -= (float)nofParticlesToCreateThisFrame;
  	
  // Update modifiers
  for (list<Modifier*>::iterator mit = mModifiers.begin(); mit != mModifiers.end(); mit++)
  {
    Modifier *m = *mit;
    m->update();
  }

  // Update particles
  for (size_t pi = 0; pi < mMaxNofParticles; pi++)
  {
    Particle *p = &mParticles[pi];

    if (p->mIsDead && nofParticlesToCreateThisFrame > 0)
    {
      defineParticle (p);
      nofParticlesToCreateThisFrame--;

      mParticleCount++;
    }

    if (!p->mIsDead)
    {
      // Update particles for modifiers
      for (list<Modifier*>::iterator mit = mModifiers.begin(); mit != mModifiers.end(); mit++)
      {
        Modifier *m = *mit;
        
        m->apply(p);
      }

      p->update();

      if (p->mIsDead)
        mParticleCount--;
    }
  }
    
}

void Emitter::draw()
{
	glEnable( GL_TEXTURE_2D );
  mParticleTexture->bind();

  glBegin(GL_QUADS);

  // Draw all particles
  for (size_t pi = 0; pi < mMaxNofParticles; pi++)
  {
    Particle *p = &mParticles[pi];

    if(!p->mIsDead)
      p->draw();
  }
    
  glEnd();
}
