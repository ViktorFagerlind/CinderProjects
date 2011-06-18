#pragma once

#include "Particle.h"
#include "Modifier.h"
#include "cinder/gl/gl.h"

#include <list>
using std::list;

class Emitter
{
public:
public:
private:
private:

  list<Particle*>  mParticles;
	list<Modifier*>  mModifiers;     // All the modifiers
	
  float 					mParticlesPerFrame;
  float 					mParticlesPerFrameCount;
  
  bool            mKilled;
  
  Emitter(std::string particleImageFile, float ppf) 
  {
    // particleImage = p.loadImage(particleImageFile);
    
    mParticlesPerFrameCount = 0;
    mParticlesPerFrame = ppf;
    mKilled = false;
  }

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
  
  void update()
  {
  	mParticlesPerFrameCount += mParticlesPerFrame;
  	
  	if (!mKilled && mParticlesPerFrameCount >= 1.0f)
  	{
	  	// create and add particles
	    for (int i=(int)mParticlesPerFrameCount; i > 0; i--)
	    {
	    	mParticles.push_back(createParticle());
	    }
	    
	    mParticlesPerFrameCount = 0.0f;
  	}
  	
    // Update particles
    for (list<Particle*>::iterator pit = mParticles.begin(); pit != mParticles.end();)
    {
      Particle *p = *pit;

      // Update modifiers
      for (list<Modifier*>::iterator mit = mModifiers.begin(); mit != mModifiers.end(); mit++)
      {
        Modifier *m = *mit;
        
        m->apply(p);
      }
      
      p->update();
      
      if (p->dead())
      {
        pit = mParticles.erase(pit);
      }
      else
        pit++;
    }
    
  }

  void draw()
  {
   	glBegin(GL_QUADS);

    //p.texture(particleImage);
  	// Draw all particles
    for (list<Particle*>::iterator it = mParticles.begin(); it != mParticles.end(); it++)
      (*it)->draw();
    
  	glEnd();
  }

  virtual Particle* createParticle() = 0; 

};