#pragma once

#include "ParticleSystem.h"

ParticleSystem::ParticleSystem()
{
}

ParticleSystem::~ParticleSystem()
{
  for (vector<Emitter*>::iterator it = mEmitters.begin(); it != mEmitters.end(); it++)
    delete (*it);
  mEmitters.clear();

  for (vector<Modifier*>::iterator it = mModifiers.begin(); it != mModifiers.end(); it++)
    delete (*it);
  mModifiers.clear();
}

void ParticleSystem::addEmitter (Emitter *const emitter)
{
  mEmitters.push_back (emitter);
}
  
void ParticleSystem::addModifier (Modifier *const modifier)
{
  mModifiers.push_back (modifier);
}

size_t ParticleSystem::getCount()
{
  size_t count = 0;

  for (vector<Emitter*>::iterator it = mEmitters.begin(); it != mEmitters.end(); it++)
    count += (*it)->getCount();
    
  return count;
}
  
void ParticleSystem::updateEmitters()
{
  for (vector<Emitter*>::iterator it = mEmitters.begin(); it != mEmitters.end();)
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

void ParticleSystem::updateModifiers()
{
  // Update modifiers
  for (vector<Modifier*>::iterator mit = mModifiers.begin(); mit != mModifiers.end(); mit++)
  {
    Modifier *m = *mit;
    m->update();

    for (vector<Emitter*>::iterator it = mEmitters.begin(); it != mEmitters.end(); it++)
    {
      Emitter *e = *it;
      e->applyModifierToParticles (m);
    }
  }
}

void ParticleSystem::update()
{
  updateEmitters();

  updateModifiers();
}

void ParticleSystem::draw()
{
  for (vector<Emitter*>::iterator it = mEmitters.begin(); it != mEmitters.end(); it++)
    //(*it)->draw();
    (*it)->drawArray();
}


