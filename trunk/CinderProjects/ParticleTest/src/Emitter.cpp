#include "Emitter.h"

Emitter::Emitter(const size_t maxNofParticles, std::string particleImageFile, const float particlesPerFrame) 
: mMaxNofParticles(maxNofParticles),
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

void Emitter::applyModifierToParticles(Modifier *modifier)
{
  for (size_t pi = 0; pi < mParticleCount; pi++)
  {
    Particle *p = &mParticles[pi];
    modifier->apply(p);
  }
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

    p->update();
  }
    
}

void Emitter::draw()
{
	glEnable( GL_TEXTURE_2D );
  mParticleTexture->bind();

  glBegin(GL_QUADS);

  // Draw all particles
  for (size_t pi = 0; pi < mParticleCount; pi++)
  {
    Particle *p = &mParticles[pi];
    p->draw();
  }
    
  glEnd();
}
