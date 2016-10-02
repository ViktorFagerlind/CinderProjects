#pragma once

#include "Particle.h"
#include "Modifier.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/Rand.h"
#include "cinder/Vector.h"

#include "Math.h"

#include "SpriteData.h"
#include "SpriteDataParser.h"
#include "BillboardParticleDrawer.h"
#include "AnimParticleDrawer.h"

#include <list>

using namespace ci;
using namespace std;

using std::list;

class Emitter
{
public:
  static vec3 getRandomVelocity  (const float minVelocity, const float maxVelocity);

  static vec3 getRandomDirection ();

public:
  // For normal emitters
  Emitter (const size_t maxNofParticles, 
           const float particlesPerFrame, 
           const vec3 position,
           const vec3 baseVelocity,
	         const float minRandVelocity, 
	         const float maxRandVelocity,
           shared_ptr<ParticleDrawerInterface> drawer);

  virtual ~Emitter();

  // A method to test if the particle system still has particles
  bool dead() {return mKilled && mParticleCount == 0;}

  void setFramesToLive (const uint32_t framesToLive) {mFramesToLive = framesToLive;}

  size_t getCount()  {return mParticleCount;}
  
  void kill() {mKilled = true;}

  void pause() {mPaused = true;}
  void unpause() {mPaused = false;}

  void applyModifierToParticles(Modifier *modifier);
  
  virtual void updateEmitter();

  void draw (const vec2 &textureSize);

  void burst (size_t nofParticles);

  void setPosition (const vec3& position)
  {
    mPosition = position;
  }

  void setRotation (const vec3& rotation)
  {
    mRotation = rotation;
  }
  
  const vector<Particle>& getParticles () {return mParticles;}

  virtual void defineParticle (Particle* particle) = 0; 


protected:
  vec3 getParticleVelocity ();

protected:
  vec3           mPosition;
  vec3           mRotation;
	vec3           mBaseVelocity;
	float           mMinRandVelocity; 
	float           mMaxRandVelocity; 

private:
  vector<Particle> mParticles;

  float 					mParticlesPerFrame;
  float 					mParticlesToCreate;

  const size_t    mMaxNofParticles;
  size_t          mParticleCount;
  
  bool            mKilled;
  bool            mPaused;

/*
  float          *mSizes;
  vec3          *mVerticies;
  GLfloat        *mTextureCoordinates;
  ColorAf        *mColors;
*/

  // gl::GlslProg    mShader;

  bool            mIsAnimated;

  uint32_t        mFramesToLive;
  shared_ptr<vector<SpriteData>> m_spriteData;

  shared_ptr<ParticleDrawerInterface> mDrawer;
};

