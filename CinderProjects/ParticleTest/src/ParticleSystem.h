#pragma once

#include "Emitter.h"

#include <list>

using namespace ci;
using std::list;

class ParticleSystem
{
public:
  ParticleSystem();

  virtual ~ParticleSystem();

  void addEmitter (Emitter *const emitter);
  
  void addModifier (Modifier *const modifier);

  size_t getCount();
  
  void update();

  void draw();

public:
  void updateEmitters();
  void updateModifiers();

private:
  list<Emitter*>  mEmitters;

	list<Modifier*> mModifiers;
};

