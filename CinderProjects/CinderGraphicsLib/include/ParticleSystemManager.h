#pragma once

#include "ParticleSystem.h"

#include <list>

using namespace ci;
using std::list;

class ParticleSystemManager
{
public:
  virtual ~ParticleSystemManager()
  {
    for (list<ParticleSystem*>::iterator it = mParticleSystems.begin(); it != mParticleSystems.end(); it++)
      delete *it;

    mParticleSystems.clear();
  }

  void addParticleSystem (ParticleSystem *const particleSystem)
  {
    mParticleSystems.push_back(particleSystem);
  }

  size_t getCount()
  {
    size_t count = 0;

    for (list<ParticleSystem*>::iterator it = mParticleSystems.begin(); it != mParticleSystems.end(); it++)
      count += (*it)->getCount();
    
    return count;
  }
  
  void update()
  {
    for (list<ParticleSystem*>::iterator it = mParticleSystems.begin(); it != mParticleSystems.end();)
    {
      ParticleSystem *ps = *it;

      ps->update();

  	  if (ps->dead())
      {
  	    it = mParticleSystems.erase (it);
        delete ps;
      }
      else
        it++;
    }
  }

  void draw()
  {
    for (list<ParticleSystem*>::iterator it = mParticleSystems.begin(); it != mParticleSystems.end(); it++)
      (*it)->draw();
  }

private:
  list<ParticleSystem*>  mParticleSystems;
};

