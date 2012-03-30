#pragma once

#include "cinder/app/KeyEvent.h"

#include "BasicObject.h"
#include "PhysicsObject.h"

#include "../../ParticleTest/src/ParticleSystemManager.h"
#include "../../ParticleTest/src/ParticleSystem.h"
#include "cinder/Rand.h"

#include "GravityField.h"
#include "../../SkyRoads/src/MovingCamera.h"

using namespace ci;
using namespace std;
using namespace ci::app;

class GameWorld
{
public:
  static GameWorld *getSingleton ();

  void keyDown (KeyEvent event);

  void setup ();

  void update ();
  void draw   ();

  static ParticleSystem *getExplosionSystem ()  {return getSingleton ()->mExposionSystem;}

  static GravityField* getGravityField ()       {return getSingleton ()->mGravityField;}

public:
  static unsigned int mScreenWidth;
  static unsigned int mScreenHeight;

private:
  PhysicsObject *mCenterObject;

  list<BasicObject *> mObjects;

  MovingCamera *mMovingCamera;

  ParticleSystemManager *mParticleSystemManager;

  ParticleSystem  *mExposionSystem;
  GravityField    *mGravityField;

  static GameWorld* theGameWorld;
};


