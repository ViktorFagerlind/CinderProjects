#include "Emitter.h"

#include "cinder/Vector.h"
#include "cinder/ImageIo.h"
#include "cinder/Rand.h"

class ImageEmitter : public Emitter 
{
public:
	ImageEmitter(const size_t maxNofParticles,
               std::string particleImageFile, 
               const float particlesPerFrame,
               std::string emitterImageFile, 
               const Vec3f& position,
							 float 	minParticleSize,
							 float 	maxParticleSize,
							 float 	minVelocity,
							 float 	maxVelocity,
							 float  width,
							 float  height,
							 float  depth) 
  : Emitter(maxNofParticles, particleImageFile, particlesPerFrame),
    mPosition(position),
		mMinParticleSize(minParticleSize),
		mMaxParticleSize(maxParticleSize),
		mMinVelocity(minVelocity),
		mMaxVelocity(maxVelocity),
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
		Vec2i imagePosition;
		
		getImagePos (imagePosition);
		
		Vec3f relativeParticlePos(((float)imagePosition.x / (float)mEmitterImage->getWidth())  * mWidth  - mWidth/2,
											        ((float)imagePosition.y / (float)mEmitterImage->getHeight()) * mHeight - mHeight/2,
											        Rand::randFloat(-mDepth, mDepth));
		
		float particleSize = Rand::randFloat(mMinParticleSize, mMaxParticleSize);

	  particle->define(relativeParticlePos + mPosition, 
	  								 particleSize, 
	  								 mMinVelocity, 
	  							   mMaxVelocity);
  }
	
private:
	void getImagePos (Vec2i& imagePosition)
	{
    ColorAf color;
		
		do
		{
			imagePosition.set (Rand::randInt(mEmitterImage->getWidth()),
			                   Rand::randInt(mEmitterImage->getHeight()));
			
      color = mEmitterImage->getPixel(imagePosition);
		}
		while((color.r + color.g + color.b) == 0);
	}

private:
  Surface	*mEmitterImage;

  Vec3f   mPosition;
  
  float 	mDepth;
  float 	mWidth;
	float 	mHeight;
	
  float 	mMinVelocity;
	float 	mMaxVelocity; 
	float 	mMinParticleSize;
	float 	mMaxParticleSize; 
};
