#pragma once

#include <Box2D/Box2d.h>

enum EntityCategory
{
  EntityCategory_OurShots_E      = 0x0001,
  EntityCategory_EnemyShots_E    = 0x0002,
  EntityCategory_Enemies_E       = 0x0004,
  EntityCategory_OurShip_E       = 0x0008
};

class ContactListener : public b2ContactListener
{
  void BeginContact (b2Contact* contact);
  
  void EndContact (b2Contact* contact);
};