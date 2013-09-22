#pragma once

#include "cinder/Vector.h"

class Emitter;
class ParticleSystem;

using namespace ci;

class ParticleSystemHelper
{
public:
  /*
  ParticleSystemHelper ();

  static ParticleSystemHelper& getSingleton ()
  {
      static ParticleSystemHelper instance;

      return instance;
  }
  */

  static Emitter* createThrustSystem ();

  static Emitter* createFlareSystem ();

  static Emitter* createMiniExplosion ();

  static Emitter* createExplosion (const Vec3f& position, const Vec3f& speed);

  static Emitter* createSparks ();
/*
private:
  ParticleSystem* m_thrustSystem;
  ParticleSystem* m_flareSystem;
  ParticleSystem* m_miniExplosionSystem;
  ParticleSystem* m_sparksSystem;
  */
};

