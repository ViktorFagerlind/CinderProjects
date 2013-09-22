#pragma once

#include "cinder/Vector.h"

#include "Weapon.h"
#include "Vessel.h"

class Emitter;

using namespace ci;
using namespace std;

//----------------------------------------------------------------------------------------------------------------------

class Engine
{
public:
  Engine ();

  virtual ~Engine ();

  void update (const float dt, const Vec2f& pos);

  void draw ();

private:
  Emitter *m_baseEmitter;
  Emitter *m_flareEmitter;
};

//----------------------------------------------------------------------------------------------------------------------

class MainVessel
{
public:
  MainVessel ();

  virtual ~MainVessel ();

  void update (const float dt, const Vec2f& touchPos);

  void draw ();

private:
  shared_ptr<Vessel> m_vessel;

  Engine             m_engine;

  shared_ptr<Weapon> m_leftLaser;
  shared_ptr<Weapon> m_rightLaser;
};

//----------------------------------------------------------------------------------------------------------------------

