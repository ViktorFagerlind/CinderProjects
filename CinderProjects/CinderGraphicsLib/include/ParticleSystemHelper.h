#pragma once

#include "cinder/Vector.h"
#include "cinder/Color.h"

using namespace ci;

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

  static Emitter* createFireBall (const Vec3f&   position, 
                                  const Vec3f&   speed, 
                                  const ColorAf& startColor, 
                                  const ColorAf& endColor,
                                  const float    size);

  static Emitter* createSparkExplosion (const Vec3f&   position, 
                                        const Vec3f&   speed, 
                                        const Colorf&  color,
                                        const float    size);

  static Emitter* createSparks ();
/*
private:
  ParticleSystem* m_thrustSystem;
  ParticleSystem* m_flareSystem;
  ParticleSystem* m_miniExplosionSystem;
  ParticleSystem* m_sparksSystem;
  */
};

