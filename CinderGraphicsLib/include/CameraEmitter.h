#include "Emitter.h"

#include "cinder/Capture.h"

#include "NewParticleDrawer.h"


class CameraEmitter : public Emitter, public Modifier
{
public:
	CameraEmitter(const size_t maxNofParticles,
               const float particlesPerFrame,
               const vec3& position,
							 float 	minParticleSize,
							 float 	maxParticleSize,
							 float 	minRandVelocity,
							 float 	maxRandVelocity,
							 float  width,
							 float  height,
               float  depth,
               shared_ptr<ParticleDrawerInterface> drawer = shared_ptr<ParticleDrawerInterface> (new NewParticleDrawer ()));

	CameraEmitter();

  void updateEmitter() override;

  void apply (Particle *const particle) override;

  void defineParticle (Particle* particle) override;
  
  SurfaceRef getImage () {return mCapturedImage;}
	
private:
	ColorAf getColorAtPosition (vec3 &position);

  bool getImagePosAndColor (ivec2& imagePosition, ColorAf& color);

private:

  float 	mDepth;
  float 	mWidth;
	float 	mHeight;
	
	float 	mMinParticleSize;
	float 	mMaxParticleSize; 

  uint32_t mCaptureWidth;
  uint32_t mCaptureHeight;
  
  CaptureRef mCapture;
  SurfaceRef mCapturedImage;
};

