#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/Vector.h"
#include "cinder/Timeline.h"

#include "MiscMath.h"
#include "ParticleSystemManager.h"
#include "ParticleSystem.h"
#include "PointEmitter.h"
#include "CommonModifier.h"
#include "ColorModifier.h"
#include "PointGravityModifier.h"

using namespace ci;

class Seed
{
public:
  Seed ()
  {
    m_position.value () = vec3 (1000, 1000, 1000);

    ParticleSystem *seedSystem  = new ParticleSystem ("../Media/Images/Particle1.jpg");

    m_seedEmitter = new PointEmitter (10000,
                                      vec3(0, 0, 0), //position
                                      1.0f,  // particles per frame
                                      0.5f,   // min size
                                      1.0f,   // max size
                                      vec3 (0, 0, 0),     // baseVelocity
                                      0.f,            // minRandVelocity
                                      0.1f);    // maxRandVelocity

    CommonModifier *cmnMod = new CommonModifier (0.9f,   // lifeChange
                                                 1,      // relativeStartSize
                                                 0.3f);  // relativeEndSize

    ColorModifier *colMod = new ColorModifier (ColorAf(0.3f, 1.0f, 0.2f, 1.0f),   // startColor
                                               ColorAf(0.3f, 1.0f, 0.2f, 0.5f), // middleColor
                                               ColorAf(0.3f, 1.0f, 0.2f, 0.5f),   // endColor
                                               0.5f);                     // middleTime

    PointGravityModifier *pgMod = new PointGravityModifier (vec3(0, 0, 0),  // position
                                                            5.0f,            // strength
                                                            0.5f,            // max strength
                                                            100.0f);         // radius

    seedSystem->addEmitter  (m_seedEmitter);
    seedSystem->addModifier (cmnMod);
    seedSystem->addModifier (colMod);
    seedSystem->addModifier (pgMod);

    ParticleSystemManager::getSingleton ().addParticleSystem (seedSystem);
  }

  void spawn ()
  {
    app::App *app = app::App::get ();

    m_position.value () = vec3 (100,100,100);
    app->timeline ().apply (&m_position, 
                            MiscMath::getRandomDirection ().normalized () * 80.f, 
                            vec3 (0,0,0), 
                            13.00f, 
                            EaseInCubic());
  }

  void update ()
  {
    m_seedEmitter->setPosition (m_position.value ());
  }

  void draw (const gl::VboMesh& mesh)
  {
    gl::pushModelView ();
    gl::enable  (GL_RESCALE_NORMAL);
    gl::translate (m_position);
    gl::scale   (0.3f, 0.3f, 0.3f);

    gl::color (0.7f, 1.f, 0.4f);
    gl::draw (mesh);

    gl::disable  (GL_RESCALE_NORMAL);
    gl::popModelView ();  
  }

public:
  Anim<vec3>     m_position;

  PointEmitter   *m_seedEmitter;
};
