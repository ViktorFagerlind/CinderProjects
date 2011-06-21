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
  Emitter(std::string particleImageFile, const float particlesPerFrame);

  virtual ~Emitter();

  // A method to test if the particle system still has particles
  bool dead() 
  {
    return mKilled && mParticleCount == 0;
  }

  void addModifier (Modifier *const modifier)
  {
  	mModifiers.push_back (modifier);
  }
  
  size_t getCount()
  {
  	return mParticleCount;
  }
  
  void kill() 
  {
  	mKilled = true;
  }
  
  void update();

  void draw();

  virtual void defineParticle (Particle* particle) = 0; 

private:
//  list<Particle*> mParticles;
  Particle       *mParticles;

	list<Modifier*> mModifiers;     // All the modifiers
	
  float 					mParticlesPerFrame;
  float 					mParticlesToCreate;

  const size_t    mMaxNofParticles;
  size_t          mParticleCount;
  
  bool            mKilled;

  gl::Texture    *mParticleTexture;
};

