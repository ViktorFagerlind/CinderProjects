//
//  Flame.cpp
//  iOsParticleTest
//
//  Created by Viktor Fägerlind on 02/10/16.
//
//

#include "Flame.h"

#include "Macros.h"
#include "ParticleSystemManager.h"
#include "ParticleSystem.h"
#include "ImageLibrary.h"
#include "Emitter.h"
#include "AreaEmitter.h"
#include "PointEmitter.h"
#include "CommonModifier.h"
#include "ColorModifier.h"
#include "PerlinModifier.h"
#include "DampingModifier.h"
#include "AnimParticleDrawer.h"


Flame::Flame()
{
  CommonModifier  *commonModifier;
  ColorModifier   *colorModifier;
  PerlinModifier  *perlinModifier;
  DampingModifier *dampingModifier;
  

  
  ////////////////////////////////////////////////////////////////////////////////////////////////////////
  //  Flame
  ///////////
  m_particleSystems.push_back (new ParticleSystem (ImageLibrary::getSingleton ().getTexture ("FireBall_BW.png")));
  
  commonModifier = new CommonModifier (1.f, 1.0f, 2.0f);
  colorModifier  = new ColorModifier  (ColorAf(0.8f,  0.8f,  1.0f,  1.0f),  //startColor
                                       ColorAf(1.0f,  0.2f,  0.2f,  0.3f),  //middleColor
                                       ColorAf(1.0f,  0.2f,  0.2f,  0.0f),  //endColor
                                       0.7f);                               //middleTime
  dampingModifier = new DampingModifier (0.025f);
  
  m_particleSystems.back()->addModifier (commonModifier);
  m_particleSystems.back()->addModifier (colorModifier);
  m_particleSystems.back()->addModifier (dampingModifier);
  
  m_emitters.push_back ((shared_ptr<Emitter>) new PointEmitter (700,                 // maxNofParticles,
                                                                vec3((float)cinder::app::getWindowWidth()/2.f, (float)cinder::app::getWindowHeight()/2.f, 0.f), // position,
                                                                0.1f,                 // particlesPerFrame,
                                                                160.f,                // minParticleSize,
                                                                180.f,                // maxParticleSize,
                                                                vec3(0, -3, 0),             // baseVelocity,
                                                                4.f,                 // minRandVelocity
                                                                5.f,                // maxRandVelocity
                                                                shared_ptr<ParticleDrawerInterface> (new AnimParticleDrawer ())));
  
  m_particleSystems.back()->addEmitterRef (m_emitters.back());
  
  ParticleSystemManager::getSingleton().addParticleSystem (m_particleSystems.back());
}

Flame::~Flame()
{
  for (auto ps : m_particleSystems)
    ps->killSystemAndEmitters ();
}

void Flame::setPosition (vec3 pos)
{
  for (const auto e : m_emitters)
    e->setPosition (pos);
}
