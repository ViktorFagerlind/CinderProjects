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

  mVerticies          = new Vec3f[mMaxNofParticles * 4];
  mTextureCoordinates = new GLfloat[mMaxNofParticles * 8];
  mColors             = new ColorAf[mMaxNofParticles * 4];

  for( size_t i=0; i < mMaxNofParticles * 8; i+=8)
  {
    mTextureCoordinates[i]   = 0;
    mTextureCoordinates[i+1] = 0;
    mTextureCoordinates[i+2] = 1;
    mTextureCoordinates[i+3] = 0;
    mTextureCoordinates[i+4] = 1;
    mTextureCoordinates[i+5] = 1;
    mTextureCoordinates[i+6] = 0;
    mTextureCoordinates[i+7] = 1;
  }

}

Emitter::~Emitter()
{
  delete [] mVerticies;
  delete [] mTextureCoordinates;
  delete [] mColors;
  delete [] mParticles;
  delete mParticleTexture;
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

void Emitter::drawArray()
{
  // Draw all particles
  for (size_t pi = 0; pi < mParticleCount; pi++)
  {
    Particle *p = &mParticles[pi];
    const size_t pi4 = pi*4;
  
	  mVerticies[pi4+0] = Vec3f(p->mPosition.x-p->mCurrentSize, p->mPosition.y-p->mCurrentSize, p->mPosition.z);
	  mVerticies[pi4+1] = Vec3f(p->mPosition.x+p->mCurrentSize, p->mPosition.y-p->mCurrentSize, p->mPosition.z);
	  mVerticies[pi4+2] = Vec3f(p->mPosition.x+p->mCurrentSize, p->mPosition.y+p->mCurrentSize, p->mPosition.z);
	  mVerticies[pi4+3] = Vec3f(p->mPosition.x-p->mCurrentSize, p->mPosition.y+p->mCurrentSize, p->mPosition.z);

    mColors[pi4]   = p->mColor;
    mColors[pi4+1] = p->mColor;
    mColors[pi4+2] = p->mColor;
    mColors[pi4+3] = p->mColor;
  }

	glEnable( GL_TEXTURE_2D );
  mParticleTexture->bind();

	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY );

	glVertexPointer( 3, GL_FLOAT, 0, mVerticies);
	glTexCoordPointer( 2, GL_FLOAT, 0, mTextureCoordinates);
	glColorPointer( 4, GL_FLOAT, 0, mColors);

	glDrawArrays (GL_QUADS, 0, mParticleCount*4);
    
	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );	
	glDisableClientState( GL_COLOR_ARRAY );
}
