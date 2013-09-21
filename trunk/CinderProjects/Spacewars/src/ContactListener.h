#pragma once

#include <Box2D/Box2d.h>

enum _entityCategory 
{
  OurShots_E      = 0x0001,
  EnemyShots_E    = 0x0002,
  Enemies_E       = 0x0004,
  OurShip_E       = 0x0008
};

class ContactListener : public b2ContactListener
{
  void BeginContact (b2Contact* contact);
  
  void EndContact (b2Contact* contact);
};