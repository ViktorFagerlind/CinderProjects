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
               const Vec3f& position,
							 float 	minParticleSize,
							 float 	maxParticleSize,
							 float 	minVelocity,
							 float 	maxVelocity,
							 float  width,
							 float  height,
							 float  depth) 
  : Emitter (maxNofParticles, particlesPerFrame, position, Vec3f::zero (), minVelocity, maxVelocity),
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
		Vec2i imagePosition;
		
		getImagePos (imagePosition);
		
		Vec3f relativeParticlePos(((float)imagePosition.x / (float)mEmitterImage->getWidth())  * mWidth  - mWidth/2,
											        ((float)imagePosition.y / (float)mEmitterImage->getHeight()) * mHeight - mHeight/2,
											        Rand::randFloat(-mDepth, mDepth));
		
		float particleSize = Rand::randFloat(mMinParticleSize, mMaxParticleSize);

	  particle->define(relativeParticlePos + mPosition, 
	  								 particleSize, 
	  								 getParticleVelocity ());
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
