#pragma once

#include "Particle.h"
#include "Modifier.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/gl.h"

#include <list>

using namespace ci;
using std::list;

class Emitter
{
public:
  Emitter(const size_t maxNofParticles, std::string particleImageFile, const float particlesPerFrame);

  virtual ~Emitter();

  // A method to test if the particle system still has particles
  bool dead() 
  {
    return mKilled && mParticleCount == 0;
  }

  size_t getCount()
  {
  	return mParticleCount;
  }
  
  void kill() 
  {
  	mKilled = true;
  }

  void applyModifierToParticles(Modifier *modifier);
  
  void update();

  void draw();

  void drawArray();

  virtual void defineParticle (Particle* particle) = 0; 

private:
  Particle       *mParticles;

  float 					mParticlesPerFrame;
  float 					mParticlesToCreate;

  const size_t    mMaxNofParticles;
  size_t          mParticleCount;
  
  bool            mKilled;

  gl::Texture    *mParticleTexture;

  Vec3f          *mVerticies;
  GLfloat        *mTextureCoordinates;
  ColorAf        *mColors;
};
