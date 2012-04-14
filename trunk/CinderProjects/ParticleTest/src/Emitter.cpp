#include "Emitter.h"

Emitter::Emitter(const size_t maxNofParticles, const float particlesPerFrame)
: mMaxNofParticles(maxNofParticles),
  mParticleCount(0)
{
  mParticlesToCreate = 0.0f;
  mParticlesPerFrame = particlesPerFrame;
  mKilled = false;

  mParticles = new Particle[mMaxNofParticles];

  mSizes              = new float[mMaxNofParticles * 4];
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

	try {
		mShader = gl::GlslProg (loadFile ("../Media/Shaders/pointSprite_vert.glsl"), loadFile ("../Media/Shaders/colorAndTextureBlend_frag.glsl"));
	}	catch (gl::GlslProgCompileExc &exc) {
		std::cout << "Shader compile error: " << std::endl;
		std::cout << exc.what();
	}	catch (...) {
		std::cout << "Unable to load shader" << std::endl;
	}

}

Emitter::~Emitter()
{
  delete [] mVerticies;
  delete [] mTextureCoordinates;
  delete [] mColors;
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
/*
  // This is how will our point sprite's size will be modified by distance from the viewer
  float quadratic[] =  { 0.0f, 0.001f, 0.0f };
  glPointParameterfvARB (GL_POINT_DISTANCE_ATTENUATION_ARB, quadratic);

  // Query for the max point size supported by the hardware
  float maxSize = 0.0f;
  glGetFloatv (GL_POINT_SIZE_MAX_ARB, &maxSize);

  // Clamp size to 100.0f or the sprites could get a little too big on some  f the newer graphic cards. 
  if( maxSize > 100.0f )
      maxSize = 100.0f;

  glPointSize (100);

  // The alpha of a point is calculated to allow the fading of points instead of shrinking them past a defined threshold size. 
  // The threshold is defined by GL_POINT_FADE_THRESHOLD_SIZE_ARB and is not clamped to the minimum and maximum point sizes.
  glPointParameterfARB( GL_POINT_FADE_THRESHOLD_SIZE_ARB, 60.0f );

  glPointParameterfARB( GL_POINT_SIZE_MIN_ARB, 1.0f );
  glPointParameterfARB( GL_POINT_SIZE_MAX_ARB, maxSize );
*/

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

	glEnableClientState( GL_COLOR_ARRAY );
	glEnableClientState (GL_VERTEX_ARRAY);

	glDrawArrays (GL_POINTS, 0, mParticleCount);

	glDisableClientState (GL_VERTEX_ARRAY);
	glDisableClientState( GL_COLOR_ARRAY );

  glDisableVertexAttribArrayARB (sizeAttributeLocation);

  glDisable (GL_VERTEX_PROGRAM_POINT_SIZE);
  glDisable (GL_POINT_SPRITE);

  mShader.unbind ();
}

