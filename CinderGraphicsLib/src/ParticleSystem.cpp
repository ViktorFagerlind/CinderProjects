#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(std::string particleImageFile)
{
  mParticleTexture = gl::Texture::create (loadImage (loadFile (particleImageFile)));
  
  mKilled          = false;
}

ParticleSystem::ParticleSystem (ImageSourceRef particleImage)
{
  mParticleTexture = gl::Texture::create (particleImage);
  mKilled          = false;
}

ParticleSystem::ParticleSystem (gl::TextureRef particleTexture)
{
  mParticleTexture = particleTexture;
  mKilled          = false;
}

ParticleSystem::~ParticleSystem()
{
  mEmitters.clear();
  mModifiers.clear();
}

void ParticleSystem::addEmitter (Emitter *const emitter)
{
  shared_ptr<Emitter> e (emitter);
  mEmitters.push_back (e);
}

void ParticleSystem::addEmitterRef (shared_ptr<Emitter> emitter)
{
  mEmitters.push_back (emitter);
}

void ParticleSystem::addModifier (Modifier *const modifier)
{
  shared_ptr<Modifier> m (modifier);
  mModifiers.push_back (m);
}

void ParticleSystem::addModifierRef (shared_ptr<Modifier> modifier)
{
  mModifiers.push_back (modifier);
}

size_t ParticleSystem::getCount()
{
  size_t count = 0;

  for (vector<shared_ptr<Emitter>>::iterator it = mEmitters.begin(); it != mEmitters.end(); it++)
    count += (*it)->getCount();
    
  return count;
}
  
void ParticleSystem::updateEmitters()
{
  for (vector<shared_ptr<Emitter>>::iterator it = mEmitters.begin(); it != mEmitters.end();)
  {
    Emitter *e = it->get ();
    e->updateEmitter();

  	if (e->dead())
  	  it = mEmitters.erase (it);
    else
      it++;
  }
}

void ParticleSystem::updateModifiers()
{
  // Update modifiers
  for (vector<shared_ptr<Modifier>>::iterator mit = mModifiers.begin(); mit != mModifiers.end(); mit++)
  {
    Modifier *m = mit->get ();
    m->updateModifier();

    for (vector<shared_ptr<Emitter>>::iterator it = mEmitters.begin(); it != mEmitters.end(); it++)
    {
      Emitter *e = it->get ();
      e->applyModifierToParticles (m);
    }
  }
}

void ParticleSystem::killSystemAndEmitters ()
{
  mKilled = true;

  for (vector<shared_ptr<Emitter>>::iterator it = mEmitters.begin(); it != mEmitters.end(); it++)
  {
    Emitter *e = it->get ();
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
  mParticleTexture->bind ();

  for (vector<shared_ptr<Emitter>>::iterator it = mEmitters.begin(); it != mEmitters.end(); it++)
  {
    Emitter *e = it->get ();
    e->draw (vec2 ((float)mParticleTexture->getWidth (), (float)mParticleTexture->getHeight ()));
  }

  mParticleTexture->unbind ();
}


