#pragma once

#include "cinder/Cinder.h"
#include "cinder/Vector.h"

#include <Box2D/Box2d.h>

#include "ContactListener.h"

#include <list>
#include <vector>

using namespace ci;
using namespace std;

class Enemy1;
class Protagonist;
class DebugDrawer;

//----------------------------------------------------------------------------------------------------------------------

class WorldObject
{
public:
  WorldObject () {};

  virtual ~WorldObject () {};

  virtual void update (const float dt) = 0;

  virtual void draw () = 0;

  virtual bool isDead () = 0;
};


//----------------------------------------------------------------------------------------------------------------------

class World
{
private:
  World ();
public:

  virtual ~World ();

  static b2World& getPhysicsWorld ();

  void setup ();

  void update (const float dt, const Vec2f& touchPos);

  void issueNewObjects ();

  void draw ();

  static World& getSingleton ();

private:
  bool isCloseToTime (const float t) const {return m_previousTime < t && t <= m_currentTime;}

  bool isCloseToTime (const vector<float> ts) const 
  {
    for (uint32_t i=0; i<ts.size(); i++)
    {
      if (isCloseToTime (ts[i]))
        return true;
    }
    return false;
  }

private:
  float                           m_currentTime;
  float                           m_previousTime;

  b2Vec2                          m_gravity;
  b2World                         m_physicsWorld;

  list<shared_ptr<WorldObject>>   m_objects;

  shared_ptr<Protagonist>         m_protagonist;

  shared_ptr<DebugDrawer>         m_debugDrawer;

  ContactListener                 m_contactListener;

  b2Body*                         m_body;


};

//----------------------------------------------------------------------------------------------------------------------

