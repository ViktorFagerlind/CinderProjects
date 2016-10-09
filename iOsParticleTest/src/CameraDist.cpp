//
//  FireComet.cpp
//  iOsParticleTest
//
//  Created by Viktor Fägerlind on 02/10/16.
//
//

#include "CameraDist.h"

#include "Macros.h"
#include "ParticleSystemManager.h"
#include "ParticleSystem.h"
#include "ImageLibrary.h"

#include "Emitter.h"
#include "AreaEmitter.h"
#include "CameraEmitter.h"
#include "PointEmitter.h"

#include "CommonModifier.h"
#include "ColorModifier.h"
#include "PerlinModifier.h"
#include "DampingModifier.h"


CameraDist::CameraDist()
{
  ParticleSystem  *particleSystem;
  Modifier        *modifier;
  
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////////
  //
  ///////////
  particleSystem = new ParticleSystem (ImageLibrary::getSingleton ().getTexture ("dot_alpha.png"), false);
  
  modifier = new CommonModifier (10.f, 1.0f, 1.0f);
  particleSystem->addModifier (modifier);
  
  modifier = new PerlinModifier (10.0f, 0.1, 0.003, 0.04f);
  particleSystem->addModifier (modifier);
  
  
  m_cameraEmitter = new CameraEmitter (14000,                 // maxNofParticles,
                               1000.f,                 // particlesPerFrame,
                               vec3((float)cinder::app::getWindowWidth()/2.f, (float)cinder::app::getWindowHeight()/2.f, 0.f), // position,
                               10.f,                // minParticleSize,
                               10.f,                // maxParticleSize,
                               0.f,                 // minRandVelocity
                               0.f,                // maxRandVelocity
                               cinder::app::getWindowWidth(), // width
                               cinder::app::getWindowHeight(), // height
                               0.f); // depth
  particleSystem->addEmitter (m_cameraEmitter);
  particleSystem->addModifier (m_cameraEmitter);
  /*
   const size_t maxNofParticles,
   const float particlesPerFrame,
   const vec3& position,
   float 	minParticleSize,
   float 	maxParticleSize,
   float 	minRandVelocity,
   float 	maxRandVelocity,
   float   width,
   float   height,
   float   depth,
   */
  
  
  ParticleSystemManager::getSingleton().addParticleSystem (particleSystem);
}

SurfaceRef CameraDist::getImage ()
{
  return m_cameraEmitter->getImage();
}




