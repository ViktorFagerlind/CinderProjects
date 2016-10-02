#pragma once

#include "Emitter.h"

#include <vector>

using namespace ci;
using namespace std;

class ParticleSystem
{
public:
  ParticleSystem (std::string particleImageFile, bool addativeBlend = true);

  ParticleSystem (ImageSourceRef particleImage, bool addativeBlend = true);

  ParticleSystem (gl::TextureRef particleTexture, bool addativeBlend = true);

  virtual ~ParticleSystem();

  void addEmitterRef (shared_ptr<Emitter> emitter);

  void addEmitter (Emitter *const emitter);
  
  void addModifierRef (shared_ptr<Modifier> modifier);

  void addModifier (Modifier *const modifier);

  size_t getCount();
  
  void killSystemAndEmitters ();

  void killSystem ();

  bool dead();

  void update();

  void draw();

  gl::TextureRef getTexture () {return mParticleTexture;}


private:
  void updateEmitters();
  void updateModifiers();

private:
  bool mKilled;

  vector<shared_ptr<Emitter>>  mEmitters;

	vector<shared_ptr<Modifier>> mModifiers;

  gl::TextureRef       mParticleTexture;
  
  bool                 mAddativeBlend;
};

