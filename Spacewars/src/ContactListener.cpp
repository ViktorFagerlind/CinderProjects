#include "ContactListener.h"
#include "Collider.h"
#include "Conversions.h"

void ContactListener::PreSolve (b2Contact* contact, const b2Manifold* oldManifold)
{
  Collider* colliderA = static_cast<Collider*> (contact->GetFixtureA()->GetBody()->GetUserData());
  Collider* colliderB = static_cast<Collider*> (contact->GetFixtureB()->GetBody()->GetUserData());

  // Disable contact if anyone says so (for shots etc...)
  if (colliderA == NULL           ||
      colliderB == NULL           ||
      !colliderA->performSolve () || 
      !colliderB->performSolve ())
  {
    contact->SetEnabled (false);
  }
}

void ContactListener::BeginContact(b2Contact* contact) 
{
  Collider* colliderA = static_cast<Collider*> (contact->GetFixtureA()->GetBody()->GetUserData());
  Collider* colliderB = static_cast<Collider*> (contact->GetFixtureB()->GetBody()->GetUserData());

  // Get contact point
  b2WorldManifold worldManifold;
  contact->GetWorldManifold(&worldManifold);
  Vec2f contactPoint = Conversions::fromPhysics (worldManifold.points[0]);

  if (colliderA != NULL && colliderB != NULL)
  {
    colliderA->collide (colliderB->getDamageOutput (), contactPoint);
    colliderB->collide (colliderA->getDamageOutput (), contactPoint);
  }
}
  
