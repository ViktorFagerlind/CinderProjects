#pragma once

#include "cinder/app/KeyEvent.h"
#include "cinder/Rand.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/Fbo.h"

#include "BloomEffect.h"
#include "BasicObject.h"
#include "BumpMaterial.h"
#include "PhongMaterial.h"
#include "PhysicsObject.h"
#include "GravityField.h"
#include "Sun.h"

#include "ParticleSystemManager.h"
#include "ParticleSystem.h"

#include "MovingCamera.h"

using namespace ci;
using namespace std;

class GameWorld
{
public:
  static GameWorld *getSingleton ();

  void keyDown (KeyEvent event);
  void mouseMove (MouseEvent event);

  void setup ();

  void update ();
  void draw ();

  static ParticleSystem *getExplosionSystem ()  {return getSingleton ()->mExposionSystem;}

  static GravityField* getGravityField ()       {return getSingleton ()->mGravityField;}

  static void setOrthoProjection ();

public:
  static uint32_t mScreenWidth;
  static uint32_t mScreenHeight;

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

  gl::Fbo       mBloomFbo;

  // Bloom attributes
  BloomEffect   *mBloomEffect;

  shared_ptr<PhongMaterial> mMeteorMaterial;
};


