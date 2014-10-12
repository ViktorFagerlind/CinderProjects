#include "VideoEmitter.h"

#include "cinder/app/AppBasic.h"
#include "cinder/CinderMath.h"
#include "cinder/qtime/QuickTime.h"

VideoEmitter::VideoEmitter (const size_t maxNofParticles,
                            const float particlesPerFrame,
                            const Vec3f& position,
							              float 	minParticleSize,
							              float 	maxParticleSize,
							              float 	minVelocity,
							              float 	maxVelocity,
							              float   width,
							              float   height,
                            float   depth,
                            shared_ptr<ParticleDrawerInterface> drawer)
                            : Emitter (maxNofParticles, particlesPerFrame, position, Vec3f::zero (), minVelocity, maxVelocity, drawer),
	mMinParticleSize(minParticleSize),
	mMaxParticleSize(maxParticleSize),
	mWidth(width),
	mHeight(height),
	mDepth(depth)
{
//  loadMovieFile (app::getOpenFilePath ());
  loadMovieFile ("I:\\Mortal Glory Productions\\Video\\2011-03-13 - Cosima\\Rendering\\Cosima.mp4");

}

VideoEmitter::~VideoEmitter()
{
}

void VideoEmitter::updateEmitter()
{
  static uint32_t count = 0;

  if ((count % 2) == 0) // Only 30 fps
  {
 	  mMovie->stepForward();
    mSurface = mMovie->getSurface ();
  }

  Emitter::updateEmitter ();

  count++;
}

void VideoEmitter::defineParticle (Particle* particle)
{
	Vec2i imagePosition;
	getImagePos (imagePosition);
		
  Vec3f relativeParticlePos(((float)imagePosition.x / (float)mMovieWidth)  * mWidth  - mWidth/2,
                            ((float)imagePosition.y / (float)mMovieHeight) * mHeight - mHeight/2,
											      Rand::randFloat(-mDepth, mDepth));
		
  relativeParticlePos.y = -relativeParticlePos.y; // Flip y-axis

	float particleSize = Rand::randFloat(mMinParticleSize, mMaxParticleSize);

	particle->define(relativeParticlePos + mPosition, 
	  								particleSize, 
                    getParticleVelocity ());

  ColorAf colorAtParticle = getColorAtParticle (particle);

  particle->mColor = colorAtParticle; // / colorAtParticle.length ();
}


ColorAf VideoEmitter::getColorAtParticle (Particle *const particle)
{
  if (!mSurface)
    return ColorAf (1,1,1,1);

  uint32_t x = (uint32_t)(((particle->mPosition.x + mWidth/2.0f)  / mWidth)  * (float)mMovieWidth)  % mMovieWidth;
  uint32_t y = (uint32_t)(((-particle->mPosition.y + mHeight/2.0f) / mHeight) * (float)mMovieHeight) % mMovieHeight; // Flip y-axis

  return mSurface.getPixel (Vec2i (x, y));
}


void VideoEmitter::apply (Particle *const particle)
{
  const float halfLife = Particle_fullLife_C / 2.0f;

  const float life = particle->getLife();

  if (life > halfLife)
    particle->mColor.a = (Particle_fullLife_C - particle->getLife()) / halfLife;
  else
    particle->mColor.a = particle->getLife() / halfLife;

  particle->mColor.a *= 0.5f;

//  particle->mPosition.z += 1.0f;

  // Interpolate towards the color
//  particle->mColor = lerp (particle->mColor, getColorAtParticle (particle), 0.5f);
}

	
void VideoEmitter::getImagePos (Vec2i& imagePosition)
{
#if 1
  const uint32_t add = 13;//Rand::randInt (10,13);

  static uint32_t x=0;
  static uint32_t y=0;

  ColorAf color;

  if (!mSurface)
  {
    imagePosition.set (Rand::randInt(mMovieWidth), Rand::randInt(mMovieHeight));
    return;
  }

  uint32_t loopCount = 0;
  for (;; y += add)
  {
    for (; x < mMovieWidth; x += add)
    {
      imagePosition.set (x, y);
      color = mSurface.getPixel(imagePosition);

      float colorValue = (color.r + color.g + color.b) * color.a;

      if ( colorValue*colorValue > Rand::randFloat () || loopCount > 100)
      {
        x += add;
        return;
      }

      loopCount++;
    }
    x -= mMovieWidth;

    if (y>=mMovieHeight)
      y -= mMovieHeight;
  }
#else
  ColorAf color;

  if (!mSurface)
  {
    imagePosition.set (Rand::randInt(mMovieWidth), Rand::randInt(mMovieHeight));
    return;
  }

  uint32_t loopCount = 0;
	do
	{
    imagePosition.set (Rand::randInt(mMovieWidth), Rand::randInt(mMovieHeight));
    color = mSurface.getPixel(imagePosition);

    loopCount++;
	}
  while(loopCount < 1000 && 
        (color.r + color.g + color.b) * color.a < Rand::randFloat ());
#endif
}

void VideoEmitter::loadMovieFile (const std::string &moviePath )
{
  app::AppBasic *app = app::AppBasic::get ();

	try {
    mMovie = &qtime::MovieSurface (moviePath);

    mMovieWidth  = mMovie->getWidth ();
	  mMovieHeight = mMovie->getHeight ();

		app->console () << "Dimensions:" << mMovie->getWidth() << " x " << mMovie->getHeight() << std::endl;
		app->console () << "Duration:  " << mMovie->getDuration() << " seconds" << std::endl;
		app->console () << "Frames:    " << mMovie->getNumFrames() << std::endl;
		app->console () << "Framerate: " << mMovie->getFramerate() << std::endl;
		app->console () << "Alpha channel: " << mMovie->hasAlpha() << std::endl;		
		app->console () << "Has audio: " << mMovie->hasAudio() << " Has visuals: " << mMovie->hasVisuals() << std::endl;

		mMovie->setLoop (true, true);
		mMovie->seekToStart ();
		mMovie->seekToTime (mMovie->getDuration() / 2);

    mMovie->setVolume (0);
		mMovie->play ();

    Sleep (1*1000);

  	mMovie->stop();
	}
	catch( ... ) {
		app->console() << "Unable to load the movie." << std::endl;
	}	
}
