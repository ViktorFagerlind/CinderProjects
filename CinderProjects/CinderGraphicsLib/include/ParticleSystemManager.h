#pragma once

#include "ParticleSystem.h"

#include <list>

using namespace ci;
using std::list;

class ParticleSystemManager
{
public:

  static ParticleSystemManager& getSingleton ()
  {
      static ParticleSystemManager instance;

      return instance;
  }

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
        ParticleSystemManager() {}
        ParticleSystemManager(ParticleSystemManager const&);  // Don't Implement.
        void operator=(ParticleSystemManager const&);         // Don't implement
private:
  list<ParticleSystem*>  mParticleSystems;
};

