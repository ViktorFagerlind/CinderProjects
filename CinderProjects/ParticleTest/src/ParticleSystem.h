#pragma once

#include "Emitter.h"

#include <vector>

using namespace ci;
using std::vector;

class ParticleSystem
{
public:
  ParticleSystem();

  virtual ~ParticleSystem();

  void addEmitter (Emitter *const emitter);
  
  void addModifier (Modifier *const modifier);

  size_t getCount();
  
  void kill();

  bool dead();

  void update();

  void draw();

private:
  void updateEmitters();
  void updateModifiers();

private:
  vector<Emitter*>  mEmitters;

	vector<Modifier*> mModifiers;
};

