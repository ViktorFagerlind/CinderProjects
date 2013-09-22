#pragma once

#include "Particle.h"
#include "Modifier.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/Rand.h"

#include "Math.h"

#include "SpriteData.h"
#include "SpriteDataParser.h"

#include <list>

using namespace ci;
using namespace std;

using std::list;

class Emitter
{
public:
  static Vec3f getRandomVelocity (const float maxVelocity);

public:
  // For normal emitters
  Emitter (const size_t maxNofParticles, const float particlesPerFrame);

  virtual ~Emitter();

  // For animated emitters
  void makeAnimated (shared_ptr<vector<SpriteData>> spriteData);

  // A method to test if the particle system still has particles
  bool dead() {return mKilled && mParticleCount == 0;}

  void setFramesToLive (const uint32_t framesToLive) {mFramesToLive = framesToLive;}

  size_t getCount()  {return mParticleCount;}
  
  void kill() {mKilled = true;}

  void pause() {mPaused = true;}
  void unpause() {mPaused = false;}

  void applyModifierToParticles(Modifier *modifier);
  
  virtual void updateEmitter();

  void draw (ci::gl::Texture *texture);

  virtual void setPosition (const Vec3f& position) {}

  virtual void defineParticle (Particle* particle) = 0; 

private:
  void drawNormal ();
  void drawPointSprite ();
  void drawAnimated (ci::gl::Texture *texture);

private:
  Particle       *mParticles;

  float 					mParticlesPerFrame;
  float 					mParticlesToCreate;

  const size_t    mMaxNofParticles;
  size_t          mParticleCount;
  
  bool            mKilled;
  bool            mPaused;

/*
  float          *mSizes;
  Vec3f          *mVerticies;
  GLfloat        *mTextureCoordinates;
  ColorAf        *mColors;
*/

  gl::GlslProg    mShader;

  bool            mIsAnimated;

  uint32_t        mFramesToLive;
  shared_ptr<vector<SpriteData>> m_spriteData;
};

