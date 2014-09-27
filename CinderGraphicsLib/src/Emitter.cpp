
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
	                const float maxRandVelocity)
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
  mFramesToLive(0xFFFFFFFF)
{
  mParticles = new Particle[mMaxNofParticles];

/*
  mSizes              = new float[mMaxNofParticles];
  mColors             = new ColorAf[mMaxNofParticles];
  mVerticies          = new Vec3f[mMaxNofParticles];
  mTextureCoordinates = new float[mMaxNofParticles*2];
*/

#if !defined (CINDER_COCOA_TOUCH)
  mShader =  ShaderHelper::loadShader("../Media/Shaders/pointSprite_vert.glsl", "../Media/Shaders/colorAndTextureBlend_frag.glsl");
#endif
}

void Emitter::makeAnimated (shared_ptr<vector<SpriteData>> spriteData)
{
  mIsAnimated   = true;
  m_spriteData  = spriteData;

  // Delete and reallocate attributes to send when drawing without point prites
  /*
  delete [] mSizes;
  delete [] mColors;
  delete [] mVerticies;
  delete [] mTextureCoordinates;

  mSizes              = NULL;
  mColors             = new ColorAf[mMaxNofParticles * 4];
  mVerticies          = new Vec3f[mMaxNofParticles   * 4];
  mTextureCoordinates = new float[mMaxNofParticles   * 8];
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
  if (mIsAnimated)
    drawAnimated (texture);
  else
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

void Emitter::drawBillboardTex (const Vec3f   &pos, 
                                const Vec2f   &scale, 
                                const GLfloat *texCoords,
                                const ColorAf &color, 
                                const float   rotationDegrees, 
                                const Vec3f   &bbRight, 
                                const Vec3f   &bbUp)
{
	Vec3f   verts[4];
  ColorAf colors[4] = {color, color, color, color};

	glVertexPointer   (3, GL_FLOAT, 0, verts);
	glTexCoordPointer (2, GL_FLOAT, 0, texCoords);
	glColorPointer    (4, GL_FLOAT, 0, colors);

	float sinA = math<float>::sin (rotationDegrees);
	float cosA = math<float>::cos (rotationDegrees);

	verts[0] = pos + bbRight * ( -1.f * scale.x * cosA -  1.f * sinA * scale.y ) + bbUp * ( -1.f * scale.x * sinA +  1.f * cosA * scale.y );
	verts[1] = pos + bbRight * ( -1.f * scale.x * cosA - -1.f * sinA * scale.y ) + bbUp * ( -1.f * scale.x * sinA + -1.f * cosA * scale.y );
	verts[2] = pos + bbRight * (  1.f * scale.x * cosA -  1.f * sinA * scale.y ) + bbUp * (  1.f * scale.x * sinA +  1.f * cosA * scale.y );
	verts[3] = pos + bbRight * (  1.f * scale.x * cosA - -1.f * sinA * scale.y ) + bbUp * (  1.f * scale.x * sinA + -1.f * cosA * scale.y );

	glDrawArrays (GL_TRIANGLE_STRIP, 0, 4);
}

void Emitter::drawBillboard (const Vec3f   &pos, 
                             const Vec2f   &scale, 
                             const ColorAf &color, 
                             const float   rotationDegrees, 
                             const Vec3f   &bbRight, 
                             const Vec3f   &bbUp)
{
	GLfloat texCoords[8] = { 0, 0, 0, 1, 1, 0, 1, 1};

  drawBillboardTex (pos, scale, texCoords, color, rotationDegrees, bbRight, bbUp);
}


void Emitter::drawNormal ()
{
  // Enable arrays
	glEnableClientState (GL_VERTEX_ARRAY);
	glEnableClientState (GL_TEXTURE_COORD_ARRAY);
	glEnableClientState (GL_COLOR_ARRAY);

  for (uint32_t i=0; i<mParticleCount; i++)
  {
    Particle *p   = &mParticles[i];

    drawBillboard (p->mPosition, 
                   Vec2f (p->mCurrentSize, p->mCurrentSize), 
                   p->mColor);
  }

  // Disable arrays
	glDisableClientState (GL_VERTEX_ARRAY);
	glDisableClientState (GL_TEXTURE_COORD_ARRAY);
	glDisableClientState (GL_COLOR_ARRAY);
}

void Emitter::drawAnimated (ci::gl::Texture *texture)
{
  GLfloat texCoords[8];

  uint32_t animationNofFrames = m_spriteData->size();

  // Enable arrays
	glEnableClientState (GL_VERTEX_ARRAY);
	glEnableClientState (GL_TEXTURE_COORD_ARRAY);
	glEnableClientState (GL_COLOR_ARRAY);

  for (uint32_t i=0; i<mParticleCount; i++)
  {
    Particle *p   = &mParticles[i];

    // Calculate current frame --------------
    uint32_t frameNr = (uint32_t)((1.0f - (p->getLife () / Particle_fullLife_C)) * animationNofFrames);
    if (frameNr >= animationNofFrames)
      frameNr = animationNofFrames - 1;

    SpriteData *frame = &m_spriteData->operator[] (frameNr);

    float xl = (float) frame->x             / (float)texture->getWidth ();
    float xr = (float)(frame->x + frame->w) / (float)texture->getWidth ();
	  float yu = (float) frame->y             / (float)texture->getHeight ();
    float yd = (float)(frame->y + frame->h) / (float)texture->getHeight ();

    // Calculate texture coordinates --------------
    // top left
    texCoords[0] = xl;
    texCoords[1] = yu;
    // top right
    texCoords[2] = xl;
    texCoords[3] = yd;
    // bottom right
    texCoords[4] = xr;
    texCoords[5] = yu;
    // bottom left
    texCoords[6] = xr;
    texCoords[7] = yd;

    drawBillboardTex (p->mPosition, 
                      Vec2f (p->mCurrentSize, p->mCurrentSize),
                      texCoords,
                      p->mColor);
  }

  // Disable arrays
	glDisableClientState (GL_VERTEX_ARRAY);
	glDisableClientState (GL_TEXTURE_COORD_ARRAY);
	glDisableClientState (GL_COLOR_ARRAY);
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
