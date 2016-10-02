#pragma once

#include <Box2D/Box2D.h>

class ParticleSystem;

using namespace cinder;

class ContactListener : public b2ContactListener
{
public:
  ContactListener ();
  
  void BeginContact(b2Contact* contact);
  
  void EndContact(b2Contact* contact);
  
  void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
  
  void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
  
private:
  void createContactEffect (vec3 point, float32 strength);
  
private:
  ParticleSystem *m_particleSystem;
};
