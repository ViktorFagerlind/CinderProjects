#pragma once

#include "Particle.h"
#include "Modifier.h"
#include "cinder/gl/gl.h"

#include <list>

using namespace std;

class Emitter
{
public:
  Emitter(std::string particleImageFile, float ppf);

  // A method to test if the particle system still has particles
  bool dead() 
  {
    return mKilled && mParticles.empty();
  }

  void addModifier (Modifier *const modifier)
  {
  	mModifiers.push_back (modifier);
  }
  
  size_t getCount()
  {
  	return mParticles.size();
  }
  
  void kill() 
  {
  	mKilled = true;
  }
  
  void update();

  void draw();

private:
  virtual Particle* createParticle() = 0; 

  list<Particle*>  mParticles;
	list<Modifier*>  mModifiers;     // All the modifiers
	
  float 					mParticlesPerFrame;
  float 					mParticlesPerFrameCount;
  
  bool            mKilled;
};

