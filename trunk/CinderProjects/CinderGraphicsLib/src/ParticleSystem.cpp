#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(std::string particleImageFile)
{
  mParticleTexture = gl::Texture (loadImage (loadFile (particleImageFile)));
  mKilled          = false;
}

ParticleSystem::ParticleSystem (ImageSourceRef particleImage)
{
  mParticleTexture = gl::Texture (particleImage);
  mKilled          = false;
}

ParticleSystem::ParticleSystem (gl::Texture particleTexture)
{
  mParticleTexture = particleTexture;
  mKilled          = false;
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
    e->updateEmitter();

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
    m->updateModifier();

    for (vector<Emitter*>::iterator it = mEmitters.begin(); it != mEmitters.end(); it++)
    {
      Emitter *e = *it;
      e->applyModifierToParticles (m);
    }
  }
}

void ParticleSystem::killSystemAndEmitters ()
{
  mKilled = true;

  for (vector<Emitter*>::iterator it = mEmitters.begin(); it != mEmitters.end(); it++)
  {
    Emitter *e = *it;
    e->kill();
  }
}

void ParticleSystem::killSystem()
{
  mKilled = true;
}

bool ParticleSystem::dead()
{
  return mKilled && mEmitters.size () == 0;
}


void ParticleSystem::update()
{
  updateEmitters();

  updateModifiers();
}

void ParticleSystem::draw()
{
	glEnable (GL_TEXTURE_2D);
  mParticleTexture.bind ();

  for (vector<Emitter*>::iterator it = mEmitters.begin(); it != mEmitters.end(); it++)
    (*it)->draw (&mParticleTexture);

  mParticleTexture.unbind ();
}


