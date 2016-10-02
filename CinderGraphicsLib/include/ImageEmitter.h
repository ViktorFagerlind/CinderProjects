#pragma once

#include "Emitter.h"

#include "cinder/Vector.h"
#include "cinder/ImageIo.h"
#include "cinder/Rand.h"

class ImageEmitter : public Emitter 
{
public:
	ImageEmitter(const size_t maxNofParticles,
               const float particlesPerFrame,
               std::string emitterImageFile, 
               const vec3& position,
							 float 	minParticleSize,
							 float 	maxParticleSize,
							 float 	minVelocity,
							 float 	maxVelocity,
							 float  width,
							 float  height,
               float  depth,
               shared_ptr<ParticleDrawerInterface> drawer = shared_ptr<ParticleDrawerInterface> (new BillboardParticleDrawer ()))
  : Emitter (maxNofParticles, particlesPerFrame, position, vec3::zero (), minVelocity, maxVelocity, drawer),
		mMinParticleSize(minParticleSize),
		mMaxParticleSize(maxParticleSize),
		mWidth(width),
		mHeight(height),
		mDepth(depth)
	{
    mEmitterImage = new Surface (loadImage (loadFile (emitterImageFile)));
  }

	~ImageEmitter()
  {
    delete mEmitterImage;
  }

  void defineParticle (Particle* particle) 
	{
		ivec2 imagePosition;
		
		getImagePos (imagePosition);
		
		vec3 relativeParticlePos(((float)imagePosition.x / (float)mEmitterImage->getWidth())  * mWidth  - mWidth/2,
											        ((float)imagePosition.y / (float)mEmitterImage->getHeight()) * mHeight - mHeight/2,
											        Rand::randFloat(-mDepth, mDepth));
		
		float particleSize = Rand::randFloat(mMinParticleSize, mMaxParticleSize);

	  particle->define(relativeParticlePos + mPosition, 
	  								 particleSize, 
	  								 getParticleVelocity ());
  }
	
private:
	void getImagePos (ivec2& imagePosition)
	{
    ColorAf color;
		
		do
		{
			imagePosition.set (Rand::randInt(mEmitterImage->getWidth()),
			                   Rand::randInt(mEmitterImage->getHeight()));
			
      color = mEmitterImage->getPixel(imagePosition);
		}
    while((color.r + color.g + color.b) * color.a == 0);
	}

private:
  Surface	*mEmitterImage;
  
  float 	mDepth;
  float 	mWidth;
	float 	mHeight;
	
	float 	mMinParticleSize;
	float 	mMaxParticleSize; 
};
