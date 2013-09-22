#include "ParticleSystemHelper.h"

#include "ImageLibrary.h"

#include "Macros.h"
#include "ParticleSystemManager.h"
#include "Emitter.h"
#include "AreaEmitter.h"
#include "PointEmitter.h"
#include "CommonModifier.h"
#include "ColorModifier.h"

using namespace ci::app;

//----------------------------------------------------------------------------------------------------------------------

Emitter* ParticleSystemHelper::createThrustSystem ()
{
  ParticleSystem *particleSystem = new ParticleSystem (ImageLibrary::getSingleton ().getTexture ("particle1.jpg"));

  AreaEmitter *emitter = new AreaEmitter (100,                // maxNofParticles,
                                          Vec3f::zero (),     // position, 
                                          2,                  // particlesPerFrame, 
 						                              7,                  // width
 						                              7,                  // height 
                                          7,                  // depth,
							                            8,                  // minParticleSize,
							                            12,                 // maxParticleSize,
							                            Vec3f (0, -6, 0),   // baseVelocity,
							                            0.5f);              // randVelocity
  particleSystem->addEmitter  (emitter);

  CommonModifier *commonModifier = new CommonModifier (4.0f, 1.0f, 0.6f);
  ColorModifier  *colorModifier  = new ColorModifier  (ColorAf(1.0f,  0.8f,  0.2f,  1.0f),  //startColor 
                                                       ColorAf(1.0f,  0.5f,  0.2f,  0.7f),  //middleColor
                                                       ColorAf(1.0f,  0.2f,  0.2f,  0.0f),  //endColor
                                                       0.5f);                               //middleTime
  particleSystem->addModifier (commonModifier);
  particleSystem->addModifier (colorModifier);

  ParticleSystemManager::getSingleton().addParticleSystem (particleSystem);

  return emitter;
}

//----------------------------------------------------------------------------------------------------------------------

Emitter* ParticleSystemHelper::createFlareSystem ()
{
  ParticleSystem *particleSystem = new ParticleSystem (ImageLibrary::getSingleton ().getTexture ("particle1.jpg"));

  PointEmitter *emitter = new PointEmitter (100,            // maxNofParticles,
                                            Vec3f::zero (), // position, 
                                            2,              // particlesPerFrame, 
                                            40,             // minParticleSize,
                                            40,             // maxParticleSize,
                                            Vec3f::zero (), // baseVelocity,
                                            0.f);           // randVelocity
  particleSystem->addEmitter  (emitter);

  CommonModifier *commonModifier = new CommonModifier (20.0f, 1.0f, 0.6f);
  ColorModifier  *colorModifier  = new ColorModifier  (ColorAf (0.8f,  0.8f,  1.0f,  0.4f),  //startColor 
                                                       ColorAf (0.8f,  0.8f,  1.0f,  0.2f),  //middleColor
                                                       ColorAf (0.8f,  0.8f,  1.0f,  0.0f),  //endColor
                                                       0.5f);                                //middleTime
  particleSystem->addModifier (commonModifier);
  particleSystem->addModifier (colorModifier);

  ParticleSystemManager::getSingleton().addParticleSystem (particleSystem);

  return emitter;
}

//----------------------------------------------------------------------------------------------------------------------

Emitter* ParticleSystemHelper::createMiniExplosion ()
{
  ParticleSystem *particleSystem = new ParticleSystem (ImageLibrary::getSingleton ().getTexture ("particle1.jpg"));

  PointEmitter *emitter = new PointEmitter (100,            // maxNofParticles,
                                            Vec3f::zero (), // position, 
                                            5,              // particlesPerFrame, 
                                            4,              // minParticleSize,
                                            8,              // maxParticleSize,
                                            Vec3f::zero (), // baseVelocity,
                                            1.5f);          // randVelocity
  emitter->setFramesToLive (2);
  particleSystem->addEmitter  (emitter);

  CommonModifier *commonModifier = new CommonModifier (4.0f, 1.0f, 0.6f);
  ColorModifier  *colorModifier  = new ColorModifier  (ColorAf(1.0f,  0.8f,  0.7f,  0.7f),  //startColor 
                                                       ColorAf(1.0f,  0.8f,  0.2f,  0.7f),  //middleColor
                                                       ColorAf(1.0f,  0.2f,  0.2f,  0.0f),  //endColor
                                                       0.5f);                               //middleTime
  particleSystem->addModifier (commonModifier);
  particleSystem->addModifier (colorModifier);

  particleSystem->killSystem ();

  ParticleSystemManager::getSingleton().addParticleSystem (particleSystem);

  return emitter;
}

//----------------------------------------------------------------------------------------------------------------------

Emitter* ParticleSystemHelper::createExplosion (const Vec3f& position, const Vec3f& speed)
{
  TextureAnimLibraryItem &libItem = TextureAnimLibrary::getSingleton ().getOrAddItem ("FireBall.png", "FireBall.xml");

  ParticleSystem *particleSystem = new ParticleSystem (libItem.m_texture);

  AreaEmitter *emitter = new AreaEmitter (20,               // maxNofParticles
                                          position,         // position
                                          4,                // particlesPerFrame,
  						                            20,               // width
  						                            20,               // height 
                                          20,               // depth,
                                          50.0f,            // min size
                                          80.0f,            // max size
                                          speed*0.5f,       // baseVelocity
                                          2.0f);            // randVelocity

  emitter->makeAnimated (libItem.m_spriteData);

  emitter->setFramesToLive (2);
  particleSystem->addEmitter  (emitter);

  CommonModifier *commonModifier = new CommonModifier (1.5f, 1.0f, 1.0f);
  ColorModifier  *colorModifier  = new ColorModifier  (ColorAf(1.0f,  1.0f,  1.0f,  0.7f),  //startColor 
                                                       ColorAf(1.0f,  1.0f,  1.0f,  0.7f),  //middleColor
                                                       ColorAf(1.0f,  1.0f,  1.0f,  0.3f),  //endColor
                                                       0.5f);                               //middleTime
  particleSystem->addModifier (commonModifier);
  particleSystem->addModifier (colorModifier);

  particleSystem->killSystem ();

  ParticleSystemManager::getSingleton().addParticleSystem (particleSystem);

  return emitter;
}

//----------------------------------------------------------------------------------------------------------------------

Emitter* ParticleSystemHelper::createSparks ()
{
  ParticleSystem *particleSystem = new ParticleSystem (ImageLibrary::getSingleton ().getTexture ("particle1.jpg"));

  PointEmitter *emitter = new PointEmitter (20,            // maxNofParticles,
                                            Vec3f::zero (), // position, 
                                            1,              // particlesPerFrame, 
                                            4,              // minParticleSize,
                                            6,              // maxParticleSize,
                                            Vec3f::zero (), // baseVelocity,
                                            2.f);           // randVelocity
  particleSystem->addEmitter  (emitter);

  CommonModifier *commonModifier = new CommonModifier (5.0f, 1.0f, 0.1f);
  ColorModifier  *colorModifier  = new ColorModifier  (ColorAf (0.8f,  0.8f,  1.0f,  1.0f),  //startColor 
                                                       ColorAf (0.8f,  0.8f,  1.0f,  0.5f),  //middleColor
                                                       ColorAf (0.8f,  0.8f,  1.0f,  0.0f),  //endColor
                                                       0.5f);                                //float middleTime)  particleSystem->addModifier (commonModifier);
  particleSystem->addModifier (commonModifier);
  particleSystem->addModifier (colorModifier);

  ParticleSystemManager::getSingleton().addParticleSystem (particleSystem);

  return emitter;
}

//----------------------------------------------------------------------------------------------------------------------

