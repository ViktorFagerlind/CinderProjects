#pragma once

#include "cinder/Vector.h"
#include "cinder/Color.h"

using namespace ci;
using namespace std;

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

  static shared_ptr<Emitter> createThrustSystem ();

  static shared_ptr<Emitter> createFlareSystem ();

  static shared_ptr<Emitter> createMiniExplosion ();

  static shared_ptr<Emitter> createFireBall (const vec3&   position, 
                                  const vec3&   speed, 
                                  const ColorAf& startColor, 
                                  const ColorAf& endColor,
                                  const float    size);

  static shared_ptr<Emitter> createSparkExplosion (const vec3&   position, 
                                        const vec3&   speed, 
                                        const Colorf&  color,
                                        const float    size);

  static shared_ptr<Emitter> createSparks ();
/*
private:
  ParticleSystem* m_thrustSystem;
  ParticleSystem* m_flareSystem;
  ParticleSystem* m_miniExplosionSystem;
  ParticleSystem* m_sparksSystem;
  */
};

