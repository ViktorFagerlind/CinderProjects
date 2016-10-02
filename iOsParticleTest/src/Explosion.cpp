//
//  Explosion.cpp
//  iOsParticleTest
//
//  Created by Viktor Fägerlind on 02/10/16.
//
//

#include "Explosion.h"

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


void Explosion::create (vec3 pos)
{
  ParticleSystem  *particleSystem;
  CommonModifier  *commonModifier;
  ColorModifier   *colorModifier;
  DampingModifier *dampingModifier;
  Emitter         *emitter;
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////////
  //  Fire
  ///////////
  particleSystem = new ParticleSystem (ImageLibrary::getSingleton ().getTexture ("Fireball 1.png"));
  
  commonModifier = new CommonModifier (2.5f, 1.0f, 2.5f);
  colorModifier  = new ColorModifier  (ColorAf(0.8f,  0.8f,  1.0f,  1.0f),  //startColor
                                       ColorAf(1.0f,  1.0f,  1.0f,  0.5f),  //middleColor
                                       ColorAf(1.0f,  1.0f,  1.0f,  0.0f),  //endColor
                                       0.2f);                               //middleTime
  dampingModifier = new DampingModifier (0.02f);
  
  particleSystem->addModifier (commonModifier);
  particleSystem->addModifier (colorModifier);
  particleSystem->addModifier (dampingModifier);
  
  emitter = new AreaEmitter  (700,                 // maxNofParticles,
                              pos, // position,
                              50.f,                 // particlesPerFrame,
                              100.f,  // width,
                              100.f, 	// height,
                              0.f,   // depth
                              100.f,                // minParticleSize,
                              150.f,                // maxParticleSize,
                              vec3(0),             // baseVelocity,
                              8.f,                 // minRandVelocity
                              10.f);                // maxRandVelocity
  
  emitter->setFramesToLive(5);
  
  particleSystem->addEmitter (emitter);
  particleSystem->killSystem();
  
  ParticleSystemManager::getSingleton().addParticleSystem (particleSystem);
  
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////////
  //  Sparks
  ///////////
  particleSystem = new ParticleSystem (ImageLibrary::getSingleton ().getTexture ("flare.png"));
  
  commonModifier = new CommonModifier (0.5f, 1.0f, 0.8f);
  colorModifier  = new ColorModifier  (ColorAf(1.0f,  1.0f,  1.0f,  1.0f),  //startColor
                                       ColorAf(1.0f,  1.0f,  1.0f,  1.0f),  //middleColor
                                       ColorAf(0.7f,  0.7f,  1.0f,  0.0f),  //endColor
                                       0.5f);                               //middleTime
  
  particleSystem->addModifier (commonModifier);
  particleSystem->addModifier (colorModifier);
  
  emitter = new PointEmitter (200,                 // maxNofParticles,
                             pos, // position,
                             10.f,                 // particlesPerFrame,
                             10.f,                // minParticleSize,
                             20.f,                // maxParticleSize,
                             vec3(0),             // baseVelocity,
                             4.f,                // minRandVelocity
                             5.f);                // maxRandVelocity
  
  emitter->setFramesToLive(5);
  particleSystem->addEmitter (emitter);
  particleSystem->killSystem();
  
  ParticleSystemManager::getSingleton().addParticleSystem (particleSystem);
  
  
}

