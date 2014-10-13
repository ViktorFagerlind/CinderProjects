#include "Emitter.h"
#include "ShaderHelper.h"

Vec3f Emitter::getRandomDirection ()
{
  float randomNormedZ = Rand::randFloat (-1, 1);
  float xyPlaneAngle = asin (randomNormedZ); 
  float zAxisAngle = Rand::randFloat (0, 2.0f * (float)M_PI);

  return Vec3f (cos (xyPlaneAngle) * cos (zAxisAngle),
                cos (xyPlaneAngle) * sin (zAxisAngle),
                randomNormedZ);
}

Vec3f Emitter::getRandomVelocity (const float minVelocity, const float maxVelocity)
{
  float randomVelocity = Rand::randFloat (minVelocity, maxVelocity);

  return randomVelocity * getRandomDirection ();
}

Vec3f Emitter::getParticleVelocity ()
{
  Vec3f rotatedBaseVelocity = mBaseVelocity;

  rotatedBaseVelocity.rotateZ (mRotation.z);
  rotatedBaseVelocity.rotateY (mRotation.y);
  rotatedBaseVelocity.rotateX (mRotation.x);

  return rotatedBaseVelocity + getRandomVelocity (mMinRandVelocity, mMaxRandVelocity);
}

Emitter::Emitter (const size_t maxNofParticles, 
                  const float particlesPerFrame, 
                  const Vec3f position,
                  const Vec3f baseVelocity,
	                const float minRandVelocity, 
	                const float maxRandVelocity,
                  shared_ptr<ParticleDrawerInterface> drawer)
: mMaxNofParticles(maxNofParticles),
  mPosition (position),
  mRotation (Vec3f::zero ()),
	mBaseVelocity (baseVelocity),
	mMinRandVelocity (minRandVelocity),
	mMaxRandVelocity (maxRandVelocity),
  mParticleCount(0),
  mIsAnimated(false),
  mPaused(false),
  mKilled(false),
  mParticlesToCreate(0.f),
  mParticlesPerFrame(particlesPerFrame),
  mFramesToLive(0xFFFFFFFF),
  mDrawer(drawer)
{
  mParticles.resize (mMaxNofParticles);

  mDrawer->createParticles (mParticles);

/*
  mSizes              = new float[mMaxNofParticles];
  mColors             = new ColorAf[mMaxNofParticles];
  mVerticies          = new Vec3f[mMaxNofParticles];
  mTextureCoordinates = new float[mMaxNofParticles*2];

#if !defined (CINDER_COCOA_TOUCH)
  mShader =  ShaderHelper::loadShader("../Media/Shaders/pointSprite_vert.glsl", "../Media/Shaders/colorAndTextureBlend_frag.glsl");
#endif
  */
}


Emitter::~Emitter()
{
/*
  delete [] mSizes;
  delete [] mColors;
  delete [] mVerticies;
  delete [] mTextureCoordinates;
*/

  // Delete all particles
  for (uint32_t i = 0; i < mParticles.size (); i++)
    delete mParticles[i];

 // delete [] mParticles;
}

void Emitter::applyModifierToParticles(Modifier *modifier)
{
  for (size_t pi = 0; pi < mParticleCount; pi++)
    modifier->apply (mParticles[pi]);
}

void Emitter::burst (size_t nofParticles)
{
  if (!mKilled)
    mParticlesToCreate += nofParticles;
}

void Emitter::updateEmitter ()
{
  if (!mPaused)
  {
    if (mFramesToLive == 0)
      kill ();
    else
      mFramesToLive--;

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
      defineParticle (mParticles[mParticleCount+pi]);
    }
    mParticleCount += nofParticlesToCreateThisFrame;
  }

  // Update particles
  for (size_t pi = 0; pi < mParticleCount; pi++)
  {
    // Move particles from back of the list if dead ones are found
    while (mParticles[pi]->mIsDead)
    {
      Particle *tmp = mParticles[pi];
      mParticles[pi] = mParticles[mParticleCount - 1];
      mParticles[mParticleCount - 1] = tmp;

      mParticleCount--;

      if (mParticleCount == 0)
        return;
    }

    mParticles[pi]->update ();
  }
    
}

void Emitter::draw (const Vec2f &textureSize)
{
  mDrawer->beforeDraw ();

  for (uint32_t i = 0; i<mParticleCount; i++)
    mDrawer->drawParticle (*mParticles[i], textureSize);

  mDrawer->afterDraw ();
}


//#if !defined (CINDER_COCOA_TOUCH)
#if 0

void Emitter::drawPointSprite()
{
  // Specify point sprite texture coordinate replacement mode for each texture unit
  glTexEnvf( GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE );

  glEnable (GL_POINT_SPRITE);
  glEnable (GL_VERTEX_PROGRAM_POINT_SIZE); // Modify size in vertex shader

  mShader.bind ();
  
  // Build arrays to draw
  for (uint32_t i=0; i<mParticleCount; i++)
  {
    Particle *p   = &mParticles[i];

    mSizes[i]     = p->mCurrentSize;
    mVerticies[i] = p->mPosition;
    mColors[i]    = p->mColor;
  }

  // prepare size attribute to send for each vertex
	int sizeAttributeLocation = mShader.getAttribLocation ("size");
  glEnableVertexAttribArrayARB (sizeAttributeLocation);
  glVertexAttribPointerARB (sizeAttributeLocation, 1, GL_FLOAT, GL_FALSE, 0, mSizes);

  // Prepare and draw array of points with color
	glVertexPointer( 3, GL_FLOAT, 0, mVerticies);
	glColorPointer( 4, GL_FLOAT, 0, mColors);

	glEnableClientState (GL_COLOR_ARRAY);
	glEnableClientState (GL_VERTEX_ARRAY);

	glDrawArrays (GL_POINTS, 0, mParticleCount);

	glDisableClientState (GL_VERTEX_ARRAY);
	glDisableClientState( GL_COLOR_ARRAY );

  glDisableVertexAttribArrayARB (sizeAttributeLocation);

  glDisable (GL_VERTEX_PROGRAM_POINT_SIZE);
  glDisable (GL_POINT_SPRITE);

  mShader.unbind ();
}
#endif
