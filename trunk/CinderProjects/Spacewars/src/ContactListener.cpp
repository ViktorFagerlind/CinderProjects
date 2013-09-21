#include "ContactListener.h"
#include "Collider.h"

void ContactListener::BeginContact(b2Contact* contact) 
{
  //check if fixture A was a ball
  Collider* colliderA = static_cast<Collider*> (contact->GetFixtureA()->GetBody()->GetUserData());
  Collider* colliderB = static_cast<Collider*> (contact->GetFixtureB()->GetBody()->GetUserData());

  if (colliderA != NULL && colliderB != NULL)
  {
    colliderA->collide (*colliderB);
    colliderB->collide (*colliderA);
  }
}
  
void ContactListener::EndContact(b2Contact* contact) 
{
}
