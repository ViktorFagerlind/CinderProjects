#include "Emitter.h"
#include "Modifier.h"

#include "cinder/Vector.h"
#include "cinder/ImageIo.h"
#include "cinder/Rand.h"

namespace cinder
{
  namespace qtime
  {
    class MovieSurface;
  }
}


class VideoEmitter : public Emitter, public Modifier 
{
public:
	VideoEmitter(const size_t maxNofParticles,
               const float particlesPerFrame,
               const vec3& position,
							 float 	minParticleSize,
							 float 	maxParticleSize,
							 float 	minVelocity,
							 float 	maxVelocity,
							 float  width,
							 float  height,
               float  depth,
               shared_ptr<ParticleDrawerInterface> drawer = shared_ptr<ParticleDrawerInterface> (new BillboardParticleDrawer ()));

	~VideoEmitter();

  void updateEmitter();

  void apply (Particle *const particle);

  void defineParticle (Particle* particle);
	
private:
	void getImagePos (ivec2& imagePosition);

  void loadMovieFile (const std::string &moviePath );

  ColorAf getColorAtParticle (Particle *const particle);

  const Surface& getSurface () {return mSurface;}

private:

  float 	mDepth;
  float 	mWidth;
	float 	mHeight;
	
  uint32_t mMovieWidth;
	uint32_t mMovieHeight;

	float 	mMinParticleSize;
	float 	mMaxParticleSize; 

  Surface mSurface;

	qtime::MovieSurface	*mMovie;
};

