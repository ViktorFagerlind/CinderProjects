#include "Emitter.h"

Emitter::Emitter(std::string particleImageFile, const float particlesPerFrame) 
: mMaxNofParticles(2000000),
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

  if (nofParticlesToCreateThisFrame + mParticleCount > mMaxNofParticles)
    nofParticlesToCreateThisFrame = mMaxNofParticles - mParticleCount;

  // update particles left to create
  mParticlesToCreate -= (float)nofParticlesToCreateThisFrame;
  	
  // Update modifiers
  for (list<Modifier*>::iterator mit = mModifiers.begin(); mit != mModifiers.end(); mit++)
  {
    Modifier *m = *mit;
    m->update();
  }

  // Remove dead particles
  /*
  for (size_t pi = 0; pi < mParticleCount; pi++)
  {
    if (mParticles[pi].mIsDead)
    {
      mParticles[pi] = mParticles[mParticleCount-1];
      mParticleCount--;
    }
  }
  */

  // Create new particles
  for (size_t pi = 0; pi < nofParticlesToCreateThisFrame; pi++)
  {
    defineParticle (&mParticles[mParticleCount+pi]);
  }
  mParticleCount += nofParticlesToCreateThisFrame;

  // Update particles
  for (size_t pi = 0; pi < mParticleCount; pi++)
  {
    Particle *p = &mParticles[pi];

    while (p->mIsDead)
    {
      *p = mParticles[mParticleCount-1];
      mParticleCount--;

      if (mParticleCount == 0)
        return;
    }

    // Update particles for modifiers
    /*
    for (list<Modifier*>::iterator mit = mModifiers.begin(); mit != mModifiers.end(); mit++)
    {
      Modifier *m = *mit;
        
      m->apply(p);
    }
    */
      
    p->setLife(p->getLife()-1.0f); // TBD

    p->update();
  }
    
}

void Emitter::draw()
{
/* profiling...
	glEnable( GL_TEXTURE_2D );
  mParticleTexture->bind();

  glBegin(GL_QUADS);
*/

  glBegin(GL_POINTS);

  // Draw all particles
  for (size_t pi = 0; pi < mParticleCount; pi++)
  {
    Particle *p = &mParticles[pi];
    p->draw();
  }
    
  glEnd();
}
