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
  //  Smoke
  ///////////
  m_particleSystems.push_back (new ParticleSystem (ImageLibrary::getSingleton ().getTexture ("smoke2.png"), false));
  
  commonModifier = new CommonModifier (.8f, 1.0f, 5.0f);
  colorModifier  = new ColorModifier  (ColorAf(1.0f,  1.0f,  1.0f,  0.04f),  //startColor
                                       ColorAf(1.0f,  1.0f,  1.0f,  0.07f),  //middleColor
                                       ColorAf(1.0f,  1.0f,  1.0f,  0.0f),  //endColor
                                       0.5f);                               //middleTime
  perlinModifier = new PerlinModifier (10.0f, 0.1, 0.003, 0.002f);
  
  m_particleSystems.back()->addModifier (commonModifier);
  m_particleSystems.back()->addModifier (colorModifier);
  m_particleSystems.back()->addModifier (perlinModifier);
  
  m_emitters.push_back ((shared_ptr<Emitter>) new AreaEmitter (1200,                 // maxNofParticles,
                                                               vec3((float)cinder::app::getWindowWidth()/2.f, (float)cinder::app::getWindowHeight()/2.f, 0.f), // position,
                                                               4.f,                 // particlesPerFrame,
                                                               50.f,  // width,
                                                               50.f, 	// height,
                                                               0.f,   // depth
                                                               50.f,                // minParticleSize,
                                                               100.f,                // maxParticleSize,
                                                               vec3(0,-2.5f,0),             // baseVelocity,
                                                               1.f,                 // minRandVelocity
                                                               4.0f));                // maxRandVelocity
  
  m_particleSystems.back()->addEmitterRef (m_emitters.back());
  
  ParticleSystemManager::getSingleton().addParticleSystem (m_particleSystems.back());
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////////
  //  Fire
  ///////////
  m_particleSystems.push_back (new ParticleSystem (ImageLibrary::getSingleton ().getTexture ("FireBall2.png")));
  
  commonModifier = new CommonModifier (2.3f, 1.0f, 1.0f);
  colorModifier  = new ColorModifier  (ColorAf(1.f,  1.f,  1.f,  0.7f),  //startColor
                                       ColorAf(1.f,  1.f,  1.f,  0.7f),  //middleColor
                                       ColorAf(1.f,  1.f,  1.f,  0.f),  //endColor
                                       0.8f);                             //middleTime
  dampingModifier = new DampingModifier (0.025f);
  
  m_particleSystems.back()->addModifier (commonModifier);
  m_particleSystems.back()->addModifier (colorModifier);
  m_particleSystems.back()->addModifier (dampingModifier);
  
  m_emitters.push_back ((shared_ptr<Emitter>) new PointEmitter (150,                 // maxNofParticles,
                                                                vec3((float)cinder::app::getWindowWidth()/2.f, (float)cinder::app::getWindowHeight()/2.f, 0.f), // position,
                                                                1.2f,                 // particlesPerFrame,
                                                                300.f,                // minParticleSize,
                                                                400.f,                // maxParticleSize,
                                                                vec3(0,0,0),             // baseVelocity,
                                                                4.f,                 // minRandVelocity
                                                                5.f,                // maxRandVelocity
                                                                shared_ptr<ParticleDrawerInterface> (new AnimParticleDrawer (8,7,50))));
  
  m_particleSystems.back()->addEmitterRef (m_emitters.back());
  
  ParticleSystemManager::getSingleton().addParticleSystem (m_particleSystems.back());
  
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////////
  //  Sparks
  ///////////
  m_particleSystems.push_back (new ParticleSystem (ImageLibrary::getSingleton ().getTexture ("flare.png")));
  
  commonModifier = new CommonModifier (1.5f, 1.0f, 0.8f);
  colorModifier  = new ColorModifier  (ColorAf(1.0f,  1.0f,  1.0f,  1.0f),  //startColor
                                       ColorAf(1.0f,  1.0f,  0.2f,  1.0f),  //middleColor
                                       ColorAf(1.0f,  0.2f,  0.2f,  0.0f),  //endColor
                                       0.8f);                               //middleTime
  perlinModifier = new PerlinModifier (10.0f, 0.03, 0.003, 0.002f);
  
  m_particleSystems.back()->addModifier (commonModifier);
  m_particleSystems.back()->addModifier (colorModifier);
  m_particleSystems.back()->addModifier (perlinModifier);
  
  m_emitters.push_back ((shared_ptr<Emitter>) new PointEmitter (100,                 // maxNofParticles,
                                                                vec3((float)cinder::app::getWindowWidth()/2.f, (float)cinder::app::getWindowHeight()/2.f, 0.f), // position,
                                                                1.f,                 // particlesPerFrame,
                                                                10.f,                // minParticleSize,
                                                                20.f,                // maxParticleSize,
                                                                vec3(0),             // baseVelocity,
                                                                2.f,                 // minRandVelocity
                                                                4.f));                // maxRandVelocity
  
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
