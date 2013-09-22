#pragma once

#include <Box2D/Box2d.h>

enum EntityCategory
{
  EntityCategory_OurShots_E      = 0x0001,
  EntityCategory_EnemyShots_E    = 0x0002,
  EntityCategory_Enemies_E       = 0x0004,
  EntityCategory_EnemySwarm_E    = 0x0008,
  EntityCategory_OurShip_E       = 0x0010
};

class ContactListener : public b2ContactListener
{
public:
  void PreSolve (b2Contact* contact, const b2Manifold* oldManifold);

  void BeginContact (b2Contact* contact);
};