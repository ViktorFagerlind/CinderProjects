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
  Engine (const Vec3f& relativePos);

  virtual ~Engine ();

  void update (const float dt, const Vessel *vessel);

  void draw ();

private:
  Emitter *m_baseEmitter;
  Emitter *m_flareEmitter;

  Vec3f    m_relativePos;
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

  shared_ptr<Lazer>             m_leftLaser;
  shared_ptr<Lazer>             m_rightLaser;
};

//----------------------------------------------------------------------------------------------------------------------

