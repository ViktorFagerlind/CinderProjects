#pragma once

#include "cinder/app/KeyEvent.h"
#include "cinder/Rand.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/Fbo.h"

#include "BloomEffect.h"
#include "BasicObject.h"
#include "BumpMaterial.h"
#include "PhysicsObject.h"
#include "GravityField.h"
#include "Sun.h"

#include "../../ParticleTest/src/ParticleSystemManager.h"
#include "../../ParticleTest/src/ParticleSystem.h"

#include "../../SkyRoads/src/MovingCamera.h"

using namespace ci;
using namespace std;

class GameWorld
{
public:
  static GameWorld *getSingleton ();

  void keyDown (KeyEvent event);

  void setup ();

  void update ();
  void draw ();

  static ParticleSystem *getExplosionSystem ()  {return getSingleton ()->mExposionSystem;}

  static ParticleSystemManager *getParticleSystemManager ()  {return getSingleton ()->mParticleSystemManager;}

  static GravityField* getGravityField ()       {return getSingleton ()->mGravityField;}

public:
  static unsigned int mScreenWidth;
  static unsigned int mScreenHeight;

private:
  shared_ptr<BumpMaterial> getBumpMaterial (const TriMesh& mesh,
                                            const string&  diffuseTexture,
                                            const string&  normalTexture);

private:
  Sun *mSun;

  list<BasicObject *> mObjects;

  MovingCamera *mMovingCamera;

  ParticleSystemManager *mParticleSystemManager;

  ParticleSystem  *mExposionSystem;
  GravityField    *mGravityField;

  static GameWorld* theGameWorld;

  gl::Fbo       mRenderFbo;

  // Bloom attributes
  BloomEffect   *mBloomEffect;
};


