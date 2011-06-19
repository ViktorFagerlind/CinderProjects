#pragma once

#include "Emitter.h"

#include <list>

using namespace ci;
using std::list;

class ParticleSystem
{
public:
  ParticleSystem()
  {
  }

  virtual ~ParticleSystem()
  {
    for (list<Emitter*>::iterator it = mEmitters.begin(); it != mEmitters.end(); it++)
      delete (*it);

    mEmitters.clear();
  }

  void addEmitter (Emitter *const emitter)
  {
  	mEmitters.push_back (emitter);
  }
  
  size_t getCount()
  {
    size_t count = 0;

    for (list<Emitter*>::iterator it = mEmitters.begin(); it != mEmitters.end(); it++)
      count += (*it)->getCount();
    
  	return count;
  }
  
  void update()
  {
    for (list<Emitter*>::iterator it = mEmitters.begin(); it != mEmitters.end();)
    {
      Emitter *e = *it;
      e->update();

  	  if (e->dead())
      {
  	  	it = mEmitters.erase (it);
        delete e;
      }
      else
        it++;
    }
  }

  void draw()
  {
    for (list<Emitter*>::iterator it = mEmitters.begin(); it != mEmitters.end(); it++)
      (*it)->draw();
  }

private:
  list<Emitter*>  mEmitters;
};

