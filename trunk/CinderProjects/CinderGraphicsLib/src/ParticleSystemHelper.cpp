#include "ParticleSystemHelper.h"

#include "ImageLibrary.h"

#include "Macros.h"
#include "ParticleSystemManager.h"
#include "Emitter.h"
#include "AreaEmitter.h"
#include "PointEmitter.h"
#include "CommonModifier.h"
#include "ColorModifier.h"
#include "DampingModifier.h"

using namespace ci::app;

//----------------------------------------------------------------------------------------------------------------------

Emitter* ParticleSystemHelper::createThrustSystem ()
{
  ParticleSystem *particleSystem = new ParticleSystem (ImageLibrary::getSingleton ().getTexture ("basic particle 2.png"));

  AreaEmitter *emitter = new AreaEmitter (100,                // maxNofParticles,
                                          Vec3f::zero (),     // position, 
                                          2,                  // particlesPerFrame, 
 						                              8,                  // width
 						                              5,                  // height 
                                          8,                  // depth,
							                            10,                 // minParticleSize,
							                            15,                 // maxParticleSize,
							                            Vec3f (0, -10, 0),  // baseVelocity,
							                            0.5f);              // randVelocity
  particleSystem->addEmitter  (emitter);

  DampingModifier *dampingModifier = new DampingModifier (0.01f);
  CommonModifier *commonModifier = new CommonModifier (6.0f, 1.0f, 0.3f);
  ColorModifier  *colorModifier  = new ColorModifier  (ColorAf (1.f, 0.7f, 0.4f, 0.7f),  // startColor 
                                                       ColorAf (1.f, 0.3f, 0.1f, 0.7f),  // middleColor
                                                       ColorAf (1.f, 0.3f, 0.3f, 0.2f),  // endColor
                                                       0.75f);                                                   // middleTime
  particleSystem->addModifier (commonModifier);
  particleSystem->addModifier (colorModifier);
  particleSystem->addModifier (dampingModifier);

  ParticleSystemManager::getSingleton().addParticleSystem (particleSystem);

  return emitter;
}

//----------------------------------------------------------------------------------------------------------------------

Emitter* ParticleSystemHelper::createFlareSystem ()
{
  ParticleSystem *particleSystem = new ParticleSystem (ImageLibrary::getSingleton ().getTexture ("basic particle 2.png"));

  PointEmitter *emitter = new PointEmitter (20,             // maxNofParticles,
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
  ParticleSystem *particleSystem = new ParticleSystem (ImageLibrary::getSingleton ().getTexture ("basic particle 2.png"));

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

Emitter* ParticleSystemHelper::createFireBall (const Vec3f&    position, 
                                                const Vec3f&   speed, 
                                                const ColorAf& startColor, 
                                                const ColorAf& endColor,
                                                const float    size)
{
  TextureAnimLibraryItem &libItem = TextureAnimLibrary::getSingleton ().getOrAddItem ("FireBall_BW.png", "FireBall_BW.xml");

  ParticleSystem *particleSystem = new ParticleSystem (libItem.m_texture);

  AreaEmitter *emitter = new AreaEmitter (20,               // maxNofParticles
                                          position,         // position
                                          4,                // particlesPerFrame,
  						                            size*20,          // width
  						                            size*20,          // height 
                                          size*20,          // depth,
                                          size*50.0f,       // min size
                                          size*80.0f,       // max size
                                          speed,            // baseVelocity
                                          size*2.0f);       // randVelocity

  emitter->makeAnimated (libItem.m_spriteData);

  emitter->setFramesToLive (2);
  particleSystem->addEmitter  (emitter);

  DampingModifier *dampingModifier = new DampingModifier (0.005f);
  CommonModifier *commonModifier = new CommonModifier (1.5f, 1.0f, 1.0f);
  ColorModifier  *colorModifier  = new ColorModifier  (startColor,                   //startColor 
                                                       (startColor + endColor)/2.f,  //middleColor
                                                       endColor,                     //endColor
                                                       0.5f);                        //middleTime
  particleSystem->addModifier (commonModifier);
  particleSystem->addModifier (colorModifier);
  particleSystem->addModifier (dampingModifier);

  particleSystem->killSystem ();

  ParticleSystemManager::getSingleton().addParticleSystem (particleSystem);

  return emitter;
}
//----------------------------------------------------------------------------------------------------------------------

Emitter* ParticleSystemHelper::createSparkExplosion (const Vec3f&   position,
                                                     const Vec3f&   speed,
                                                     const Colorf&  color,
                                                     const float    size)
{
  ParticleSystem *particleSystem = new ParticleSystem (ImageLibrary::getSingleton ().getTexture ("basic particle 2.png"));

  PointEmitter *emitter = new PointEmitter (100,            // maxNofParticles,
                                            position,       // position, 
                                            size*15,        // particlesPerFrame, 
                                            size*2,         // minParticleSize,
                                            size*4,         // maxParticleSize,
                                            speed,          // baseVelocity,
                                            size*10.0f);    // randVelocity
  emitter->setFramesToLive (2);

  particleSystem->addEmitter  (emitter);

  DampingModifier *dampingModifier = new DampingModifier (0.005f);
  CommonModifier  *commonModifier = new CommonModifier (1.3f, 1.0f, 0.1f);

  ColorAf startColor  (color.r, color.g, color.b, 1.0f);
  ColorAf middleColor (color.r, color.g, color.b, 0.8f);
  ColorAf endColor    (color.r, color.g, color.b, 0.2f);
  ColorModifier  *colorModifier  = new ColorModifier  (startColor,    // startColor 
                                                       middleColor,   // middleColor
                                                       endColor,      // endColor
                                                       0.8f);         // float middleTime)  particleSystem->addModifier (commonModifier);
  particleSystem->addModifier (commonModifier);
  particleSystem->addModifier (colorModifier);
  particleSystem->addModifier (dampingModifier);

  particleSystem->killSystem ();

  ParticleSystemManager::getSingleton().addParticleSystem (particleSystem);

  return emitter;
}


//----------------------------------------------------------------------------------------------------------------------

Emitter* ParticleSystemHelper::createSparks ()
{
  ParticleSystem *particleSystem = new ParticleSystem (ImageLibrary::getSingleton ().getTexture ("basic particle 2.png"));

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

