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

class ProtagonistVessel : public Vessel
{
public:
  ProtagonistVessel (const VesselDef& vesselDef)
  : Vessel (vesselDef)
  {}

  virtual ~ProtagonistVessel() {}

  virtual float getDamageOutput () const {return 100.f;}

  virtual void explode () {}; // Not implemented yet...
};

//----------------------------------------------------------------------------------------------------------------------

class Protagonist
{
public:
  Protagonist ();

  virtual ~Protagonist ();

  void update (const float dt, const Vec2f& touchPos);

  void drawSolid ();

  void drawTransparent ();

private:
  shared_ptr<ProtagonistVessel> m_vessel;

  Engine                        m_engine;

  shared_ptr<Weapon>            m_leftLaser;
  shared_ptr<Weapon>            m_rightLaser;
};

//----------------------------------------------------------------------------------------------------------------------

