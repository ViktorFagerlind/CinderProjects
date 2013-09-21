#pragma once

#include "Emitter.h"

#include <vector>

using namespace ci;
using std::vector;

class ParticleSystem
{
public:
  ParticleSystem(std::string particleImageFile);

  ParticleSystem (ImageSourceRef particleImage);

  virtual ~ParticleSystem();

  void addEmitter (Emitter *const emitter);
  
  void addModifier (Modifier *const modifier);

  size_t getCount();
  
  void kill();

  bool dead();

  void update();

  void draw();

  gl::Texture *getTexture () {return mParticleTexture;}


private:
  void updateEmitters();
  void updateModifiers();

private:
  bool mKilled;

  vector<Emitter*>  mEmitters;

	vector<Modifier*> mModifiers;

  gl::Texture      *mParticleTexture;
};

