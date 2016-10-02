#include <Box2D/Box2D.h>

#include "ParticleSystemHelper.h"

#include "ContactListener.h"
#include "ImageLibrary.h"

#include "Macros.h"
#include "ParticleSystemManager.h"
#include "ParticleSystem.h"
#include "Emitter.h"
#include "AreaEmitter.h"
#include "PointEmitter.h"
#include "CommonModifier.h"
#include "ColorModifier.h"
#include "DampingModifier.h"

#include "cinder/Log.h"

using namespace ci::app;

ContactListener::ContactListener ()
{
  m_particleSystem = new ParticleSystem (ImageLibrary::getSingleton ().getTexture ("basic particle 2.png"));
  
  CommonModifier *commonModifier = new CommonModifier (6.0f, 1.0f, 0.4f);
  ColorModifier  *colorModifier  = new ColorModifier  (ColorAf(1.0f,  0.1f,  0.1f,  1.0f),  //startColor
                                                       ColorAf(1.0f,  0.1f,  1.0f,  0.4f),  //middleColor
                                                       ColorAf(0.1f,  0.1f,  1.0f,  0.0f),  //endColor
                                                       0.5f);                               //middleTime
  DampingModifier *dampingModifier = new DampingModifier (0.5f);
  
  m_particleSystem->addModifier (commonModifier);
  m_particleSystem->addModifier (colorModifier);
  m_particleSystem->addModifier (dampingModifier);
  
  ParticleSystemManager::getSingleton().addParticleSystem (m_particleSystem);
}

void ContactListener::createContactEffect (vec3 point, float32 strength)
{
  static uint32_t count = 0UL;
  
  shared_ptr<Emitter> emitter (new PointEmitter (50,                  // maxNofParticles,
                                                 point,               // position,
                                                 2.f + strength,      // particlesPerFrame,
                                                 20.f+strength*10.f,  // minParticleSize,
                                                 20.f+strength*10.f,  // maxParticleSize,
                                                 vec3(0),             // baseVelocity,
                                                 0.f,                 // minRandVelocity
                                                 0.1f*strength));     // maxRandVelocity
  emitter->setFramesToLive (2);

  count++;
  if (count %10 == 0)
    CI_LOG_V( "Emitter creation count: " << count << " Nof particles: " << m_particleSystem->getCount());

  
  m_particleSystem->addEmitterRef (emitter);
}

void ContactListener::BeginContact(b2Contact* contact)
{
}
  
void ContactListener::EndContact(b2Contact* contact)
{
}

void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
}

void ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
  float threasholdImpact = 1.f;
  
  //int numPoints = contact->GetManifold()->pointCount;
  
  b2WorldManifold worldManifold;
  contact->GetWorldManifold( &worldManifold );
  
  float32 effectStrength = impulse->normalImpulses[0] - threasholdImpact;
  
  if (effectStrength > 1.f)
    createContactEffect (vec3 (worldManifold.points[0].x, worldManifold.points[0].y, 0.f), effectStrength);
}

/*
//----------------------------------------------------------------------------------------------------------------------

shared_ptr<Emitter> ParticleSystemHelper::createThrustSystem ()
{
  ParticleSystem *particleSystem = new ParticleSystem (ImageLibrary::getSingleton ().getTexture ("basic particle 2.png"));


  shared_ptr<Emitter> emitter (new AreaEmitter (100,                // maxNofParticles,
                                          vec3::zero (),     // position, 
                                          2,                  // particlesPerFrame, 
 						                              8,                  // width
 						                              5,                  // height 
                                          8,                  // depth,
							                            10,                 // minParticleSize,
							                            15,                 // maxParticleSize,
							                            vec3 (0, -10, 0),  // baseVelocity,
							                            0.5f));              // randVelocity
  particleSystem->addEmitterRef  (emitter);

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

shared_ptr<Emitter> ParticleSystemHelper::createFlareSystem ()
{
  ParticleSystem *particleSystem = new ParticleSystem (ImageLibrary::getSingleton ().getTexture ("basic particle 2.png"));

  shared_ptr<Emitter> emitter (new PointEmitter (20,             // maxNofParticles,
                                            vec3::zero (), // position, 
                                            2,              // particlesPerFrame, 
                                            40,             // minParticleSize,
                                            40,             // maxParticleSize,
                                            vec3::zero (), // baseVelocity,
                                            0.f,            // minRandVelocity
                                            0.f));           // maxRandVelocity
  particleSystem->addEmitterRef  (emitter);

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

shared_ptr<Emitter> ParticleSystemHelper::createMiniExplosion ()
{
  ParticleSystem *particleSystem = new ParticleSystem (ImageLibrary::getSingleton ().getTexture ("basic particle 2.png"));

  shared_ptr<Emitter> emitter (new PointEmitter (100,            // maxNofParticles,
                                            vec3::zero (), // position, 
                                            5,              // particlesPerFrame, 
                                            4,              // minParticleSize,
                                            8,              // maxParticleSize,
                                            vec3::zero (), // baseVelocity,
                                            0.f,            // minRandVelocity
                                            1.5f));          // maxRandVelocity
  emitter->setFramesToLive (2);
  particleSystem->addEmitterRef  (emitter);

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

shared_ptr<Emitter> ParticleSystemHelper::createFireBall (const vec3&    position, 
                                                const vec3&   speed, 
                                                const ColorAf& startColor, 
                                                const ColorAf& endColor,
                                                const float    size)
{
  TextureAnimLibraryItem &libItem = TextureAnimLibrary::getSingleton ().getOrAddItem ("FireBall_BW.png", "FireBall_BW.xml");

  ParticleSystem *particleSystem = new ParticleSystem (libItem.m_texture);

  shared_ptr<AnimParticleDrawer> animDrawer = shared_ptr<AnimParticleDrawer> (new AnimParticleDrawer (libItem.m_spriteData));

  shared_ptr<Emitter> emitter (new AreaEmitter (20,               // maxNofParticles
                                                position,         // position
                                                4,                // particlesPerFrame,
                                                size * 20,          // width
                                                size * 20,          // height 
                                                size * 20,          // depth,
                                                size*50.0f,       // min size
                                                size*80.0f,       // max size
                                                speed,            // baseVelocity
                                                size*2.0f,        // randVelocity
                                                animDrawer));


  emitter->setFramesToLive (2);
  particleSystem->addEmitterRef  (emitter);

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

shared_ptr<Emitter> ParticleSystemHelper::createSparkExplosion (const vec3&   position,
                                                     const vec3&   speed,
                                                     const Colorf&  color,
                                                     const float    size)
{
  ParticleSystem *particleSystem = new ParticleSystem (ImageLibrary::getSingleton ().getTexture ("basic particle 2.png"));

  shared_ptr<Emitter> emitter (new PointEmitter (100,            // maxNofParticles,
                                            position,       // position, 
                                            size*15,        // particlesPerFrame, 
                                            size*2,         // minParticleSize,
                                            size*4,         // maxParticleSize,
                                            speed,          // baseVelocity,
                                            0.f,            // minRandVelocity
                                            size*10.0f));    // maxRandVelocity
  emitter->setFramesToLive (2);

  particleSystem->addEmitterRef (emitter);

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

shared_ptr<Emitter> ParticleSystemHelper::createSparks ()
{
  ParticleSystem *particleSystem = new ParticleSystem (ImageLibrary::getSingleton ().getTexture ("basic particle 2.png"));

  shared_ptr<Emitter> emitter (new PointEmitter (20,            // maxNofParticles,
                                            vec3::zero (), // position, 
                                            1,              // particlesPerFrame, 
                                            4,              // minParticleSize,
                                            6,              // maxParticleSize,
                                            vec3::zero (), // baseVelocity,
                                            0.f,            // minRandVelocity
                                            2.f));           // maxRandVelocity
  particleSystem->addEmitterRef (emitter);

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

*/