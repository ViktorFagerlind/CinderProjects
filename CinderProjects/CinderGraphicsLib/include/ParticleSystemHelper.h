#pragma once

class Emitter;
class ParticleSystem;

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

  static Emitter* createSparks ();
/*
private:
  ParticleSystem* m_thrustSystem;
  ParticleSystem* m_flareSystem;
  ParticleSystem* m_miniExplosionSystem;
  ParticleSystem* m_sparksSystem;
  */
};

