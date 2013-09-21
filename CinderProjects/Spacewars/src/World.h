#pragma once

#include "cinder/Cinder.h"
#include <Box2D/Box2d.h>

#include "ContactListener.h"

#include <list>

using namespace ci;
using namespace std;

class EnemyVessel;
class DebugDrawer;

//----------------------------------------------------------------------------------------------------------------------

class World
{
private:
  World ();
public:

  virtual ~World ();

  static b2World& getPhysicsWorld ();

  void update (float dt);

  void draw ();

  static World& getSingleton ();

private:

  uint32_t                        m_previousSecond;

  b2Vec2                          m_gravity;
  b2World                         m_physicsWorld;

  list<shared_ptr<EnemyVessel>>   m_enemies;

  shared_ptr<DebugDrawer>         m_debugDrawer;

  ContactListener                 m_contactListener;

  b2Body*                         m_body;
};

//----------------------------------------------------------------------------------------------------------------------

