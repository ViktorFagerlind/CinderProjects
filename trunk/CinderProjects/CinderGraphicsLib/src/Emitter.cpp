
#include "Emitter.h"
#include "ShaderHelper.h"

Vec3f Emitter::getRandomVelocity (const float maxVelocity)
{
  float randomNormedZ = Rand::randFloat (-1, 1);
  float xyPlaneAngle = asin (randomNormedZ); 
  float zAxisAngle = Rand::randFloat (0, 2.0f * (float)M_PI);

  float randomVelocity = Rand::randFloat (0.0f, maxVelocity);

  return Vec3f (randomVelocity * cos (xyPlaneAngle) * cos (zAxisAngle),
                randomVelocity * cos (xyPlaneAngle) * sin (zAxisAngle),
                randomVelocity * randomNormedZ);
}

Emitter::Emitter(const size_t maxNofParticles, const float particlesPerFrame)
: mMaxNofParticles(maxNofParticles),
  mParticleCount(0),
  mIsAnimated(false),
  mPaused(false),
  mKilled(false),
  mParticlesToCreate(0.f),
  mParticlesPerFrame(particlesPerFrame),
  mFramesToLive(0xFFFFFFFF)
{

  mParticles = new Particle[mMaxNofParticles];

  mSizes              = new float[mMaxNofParticles];
  mColors             = new ColorAf[mMaxNofParticles];
  mVerticies          = new Vec3f[mMaxNofParticles];
  mTextureCoordinates = new float[mMaxNofParticles*2];

#if !defined (CINDER_COCOA_TOUCH)
  mShader =  ShaderHelper::loadShader("../Media/Shaders/pointSprite_vert.glsl", "../Media/Shaders/colorAndTextureBlend_frag.glsl");
#endif
}

void Emitter::makeAnimated (std::string animationXmlPath)
{
  mIsAnimated = true;

  // Delete and reallocate attributes to send when drawing without point prites
  delete [] mSizes;
  delete [] mColors;
  delete [] mVerticies;
  delete [] mTextureCoordinates;

  mSizes              = NULL;
  mColors             = new ColorAf[mMaxNofParticles * 4];
  mVerticies          = new Vec3f[mMaxNofParticles   * 4];
  mTextureCoordinates = new float[mMaxNofParticles   * 8];

  m_spriteData = SpriteDataParser::parseSpriteData(animationXmlPath, 
                                                   SpriteSheet::FORMAT_TEXTUREPACKER_GENERIC_XML);
}

Emitter::~Emitter()
{
  delete [] mSizes;
  delete [] mColors;
  delete [] mVerticies;
  delete [] mTextureCoordinates;

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
      defineParticle (&mParticles[mParticleCount+pi]);
    }
    mParticleCount += nofParticlesToCreateThisFrame;
  }

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

void Emitter::draw (ci::gl::Texture *texture)
{
//#if defined (CINDER_COCOA_TOUCH)
  drawNormal ();
/*
#else
  if (mIsAnimated)
    drawAnimated (texture);
  else
    drawPointSprite ();
#endif
*/
}

void Emitter::drawNormal ()
{
  // Build arrays to draw
  const float xl = 0.f;
  const float xr = 1.f;
	const float yu = 0.f;
  const float yd = 1.f;

  // top left
  mTextureCoordinates[0] = xl;
  mTextureCoordinates[1] = yu;
  // top right
  mTextureCoordinates[2] = xr;
  mTextureCoordinates[3] = yu;
  // bottom right
  mTextureCoordinates[4] = xr;
  mTextureCoordinates[5] = yd;

  // top left
  mTextureCoordinates[6] = xl;
  mTextureCoordinates[7] = yu;
  // bottom right
  mTextureCoordinates[8] = xr;
  mTextureCoordinates[9] = yd;
  // bottom left
  mTextureCoordinates[10] = xl;
  mTextureCoordinates[11] = yd;

  // Set array pointers
	glVertexPointer   (3, GL_FLOAT, 0, mVerticies);
	glTexCoordPointer (2, GL_FLOAT, 0, mTextureCoordinates );
	glColorPointer    (4, GL_FLOAT, 0, mColors);

  // Enable arrays
	glEnableClientState (GL_VERTEX_ARRAY);
	glEnableClientState (GL_TEXTURE_COORD_ARRAY);
	glEnableClientState (GL_COLOR_ARRAY);

  for (uint32_t i=0; i<mParticleCount; i++)
  {
    Particle *p   = &mParticles[i];

    mColors[0]    = p->mColor;
    mColors[1]    = p->mColor;
    mColors[2]    = p->mColor;
    mColors[3]    = p->mColor;
    mColors[4]    = p->mColor;
    mColors[5]    = p->mColor;

    mVerticies[0] = p->mPosition + Vec3f (-p->mCurrentSize,  p->mCurrentSize, 0);
    mVerticies[1] = p->mPosition + Vec3f (p->mCurrentSize,   p->mCurrentSize, 0);
    mVerticies[2] = p->mPosition + Vec3f (p->mCurrentSize,  -p->mCurrentSize, 0);

    mVerticies[3] = p->mPosition + Vec3f (-p->mCurrentSize,  p->mCurrentSize, 0);
    mVerticies[4] = p->mPosition + Vec3f (p->mCurrentSize,  -p->mCurrentSize, 0);
    mVerticies[5] = p->mPosition + Vec3f (-p->mCurrentSize, -p->mCurrentSize, 0);

    // Draw arrays
	  glDrawArrays (GL_TRIANGLES, 0, 6);
  }
  
  // Disable arrays
	glDisableClientState (GL_VERTEX_ARRAY);
	glDisableClientState (GL_TEXTURE_COORD_ARRAY);
	glDisableClientState (GL_COLOR_ARRAY);
}

#if !defined (CINDER_COCOA_TOUCH)

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

void Emitter::drawAnimated (ci::gl::Texture *texture)
{
  uint32_t animationNofFrames = m_spriteData.size();

  // Build arrays to draw
  for (uint32_t i=0; i<mParticleCount; i++)
  {
    Particle *p   = &mParticles[i];

    mColors[i*4]      = p->mColor;
    mColors[i*4+1]    = p->mColor;
    mColors[i*4+2]    = p->mColor;
    mColors[i*4+3]    = p->mColor;

    mVerticies[i*4]   = p->mPosition + Vec3f (-p->mCurrentSize,  p->mCurrentSize, 0);
    mVerticies[i*4+1] = p->mPosition + Vec3f (p->mCurrentSize,   p->mCurrentSize, 0);
    mVerticies[i*4+2] = p->mPosition + Vec3f (p->mCurrentSize,  -p->mCurrentSize, 0);
    mVerticies[i*4+3] = p->mPosition + Vec3f (-p->mCurrentSize, -p->mCurrentSize, 0);

    uint32_t frameNr = (uint32_t)((1.0f - (p->getLife () / Particle_fullLife_C)) * animationNofFrames);
    if (frameNr >= animationNofFrames)
      frameNr = animationNofFrames - 1;

    SpriteData *frame = &m_spriteData[frameNr];
    float xl = (float) frame->x             / (float)texture->getWidth ();
    float xr = (float)(frame->x + frame->w) / (float)texture->getWidth ();
	  float yu = (float) frame->y             / (float)texture->getHeight ();
    float yd = (float)(frame->y + frame->h) / (float)texture->getHeight ();

    // top left
    mTextureCoordinates[i*8]   = xl;
    mTextureCoordinates[i*8+1] = yu;
    // top right
    mTextureCoordinates[i*8+2] = xr;
    mTextureCoordinates[i*8+3] = yu;
    // bottom right
    mTextureCoordinates[i*8+4] = xr;
    mTextureCoordinates[i*8+5] = yd;
    // bottom left
    mTextureCoordinates[i*8+6] = xl;
    mTextureCoordinates[i*8+7] = yd;
  }
  
  // Set array pointers
	glVertexPointer   (3, GL_FLOAT, 0, mVerticies);
	glTexCoordPointer (2, GL_FLOAT, 0, mTextureCoordinates );
	glColorPointer    (4, GL_FLOAT, 0, mColors);

  // Enable arrays
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY );

  // Draw arrays
	glDrawArrays (GL_QUADS, 0, mParticleCount*4);

  // Disable arrays
	glDisableClientState (GL_VERTEX_ARRAY);
	glDisableClientState (GL_TEXTURE_COORD_ARRAY);
	glDisableClientState (GL_COLOR_ARRAY);
}

#endif