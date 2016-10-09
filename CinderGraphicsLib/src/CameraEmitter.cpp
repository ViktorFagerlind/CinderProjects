#include "CameraEmitter.h"

#include "cinder/app/App.h"
#include "cinder/Capture.h"
#include "cinder/Log.h"

CameraEmitter::CameraEmitter (const size_t maxNofParticles,
                            const float particlesPerFrame,
                            const vec3& position,
							              float 	minParticleSize,
							              float 	maxParticleSize,
							              float 	minRandVelocity,
							              float 	maxRandVelocity,
							              float   width,
							              float   height,
                            float   depth,
                            shared_ptr<ParticleDrawerInterface> drawer)
                            : Emitter (maxNofParticles, particlesPerFrame, position, vec3(0), minRandVelocity, maxRandVelocity, drawer),
	mMinParticleSize(minParticleSize),
	mMaxParticleSize(maxParticleSize),
	mWidth(width),
	mHeight(height),
	mDepth(depth)
{
  try
  {
    mCapture = Capture::create( 640, 480 );
    mCapture->start();
  }
  catch( ci::Exception &exc )
  {
    CI_LOG_EXCEPTION( "Failed to init capture ", exc );
  }
  
  while (!mCapture->checkNewFrame());
  
  mCapturedImage  = mCapture->getSurface();
  
  mCaptureWidth   = mCapturedImage->getWidth();
  mCaptureHeight  = mCapturedImage->getHeight();
}


void CameraEmitter::updateEmitter()
{
  if (mCapture->checkNewFrame())
  {
    mCapturedImage = mCapture->getSurface();
  }
  
  Emitter::updateEmitter ();
}

void CameraEmitter::defineParticle (Particle* particle)
{
	ivec2   imagePosition;
  ColorAf color;
	getImagePosAndColor (imagePosition, color);
		
  vec3 relativeParticlePos (((float)imagePosition.y / (float)mCaptureHeight)  * mWidth  - mWidth/2,
                            ((float)imagePosition.x / (float)mCaptureWidth) * mHeight - mHeight/2,
											      Rand::randFloat(-mDepth, mDepth));
		
  relativeParticlePos.x = -relativeParticlePos.x;

	float particleSize = Rand::randFloat(mMinParticleSize, mMaxParticleSize);

	particle->define(relativeParticlePos + mPosition, 
	  								particleSize, 
                    getParticleVelocity ());
  particle->mColor = color;
}


ColorAf CameraEmitter::getColorAtPosition (vec3 &position)
{
  if (!mCapturedImage)
    return ColorAf (1,1,1,1);

  uint32_t x = (uint32_t)(((position.y + mWidth/2.0f)  / mWidth)  * (float)mCaptureWidth)  % mCaptureWidth;
  uint32_t y = (uint32_t)(((position.x + mHeight/2.0f) / mHeight) * (float)mCaptureHeight) % mCaptureHeight; // Flip y-axis

  return mCapturedImage->getPixel (ivec2 (x, y));
}


void CameraEmitter::apply (Particle *const particle)
{
  const float halfLife = Particle_fullLife_C / 2.0f;

  const float life = particle->getLife();

  if (life > halfLife)
    particle->mColor.a = (Particle_fullLife_C - particle->getLife()) / halfLife;
  else
    particle->mColor.a = particle->getLife() / halfLife;

  //particle->mColor.a *= 0.5f;

//  particle->mPosition.z += 1.0f;

  // Interpolate towards the color
//  particle->mColor = lerp (particle->mColor, getColorAtParticle (particle), 0.5f);
}

	
bool CameraEmitter::getImagePosAndColor (ivec2& imagePosition, ColorAf& color)
{
  static uint32_t x=0;
  static uint32_t y=0;
  
  const uint32_t add = Rand::randInt (10,13);

  x += add;
  
  if (x>=mCaptureWidth)
  {
    x -= mCaptureWidth;
    y += add;
  }
  
  if (y>=mCaptureHeight)
    y -= mCaptureHeight;
  
  imagePosition = ivec2 (x, y);
  color = mCapturedImage->getPixel(imagePosition);
  
  return true;
}


