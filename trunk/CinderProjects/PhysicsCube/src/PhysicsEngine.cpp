#include "cinder/gl/gl.h"
#include "PhysicsEngine.h"
#include "BoundingGeometry.h"
#include "CollisionDetection.h"
#include "math.h"

using namespace ci;
using namespace std;

PhysicsEngine::PhysicsEngine()
{
  cube = new DynamicObject(1.0f, Vec3f(0, 0, 0), 70.0f, 70.0f, 70.0f);
  cube->setPosition(Vec3f(300, 500, 0));

  plane = new StaticObject(1.0f, Vec3f(0, 0, 0), 70.0f, 70.0f, Vec3f(0, 0, 100), Vec3f(100, 0, 0), Vec3f(0, 0, 0));

  sphere = new DynamicObject(1.0f, Vec3f(0, 0, 0), 35.0f);
  sphere->setPosition(Vec3f(100, 0, 0));

  mTimer.start ();
  mLastTimeStamp = (float)mTimer.getSeconds ();
};

void PhysicsEngine::update()
{
    // Calculate time
  float newTime = (float)mTimer.getSeconds ();
  float deltaTime = newTime - mLastTimeStamp;
  mLastTimeStamp = newTime;


  // 1) Check if next position is valid:
 resolveCollisions();


  // 2) If valid, update positions and orientations

  /* 2) Else: 1) determine collision time
          2) determine collision response (impulse)
          3) apply impulse
          4) update position and do 1)-3) until no collision.
  */

  //Vec3f collisionPoint = getCollisionPoint();

 cube->applyForce(Vec3f(0, -50, 0));
 
  cube->update(deltaTime);
  plane->update(deltaTime);
  sphere->update(deltaTime);
}

void PhysicsEngine::draw()
{
  cube->draw();
  plane->draw();
  sphere->draw();
}

State PhysicsEngine::getState(PhysicsObject physicsObject)
{
  return physicsObject.mState;
}
  
void  PhysicsEngine::setState(PhysicsObject physicsObject)
{

}

void PhysicsEngine::applyForce(PhysicsObject physicsObject, Vec3f force)
{

}
  
void PhysicsEngine::applyTorque(PhysicsObject physicsObject, Vec3f pointOfAttack, Vec3f force)
{
  
}

void PhysicsEngine::resolveCollisions ()
{
  PhysicsObject *objects[] = {cube, plane, sphere};
  uint32_t nofObjets = 3;


  for (int i=0; i<nofObjets-1; i++)
  {
    for (int j=i+1; j<nofObjets; j++)
    {
      if (CollisionDetection::isCollision (objects[i]->mBoundingGeometry, objects[j]->mBoundingGeometry))
      {
        Vec3f normal, point;

        CollisionDetection::getCollisionPoint (objects[i]->mBoundingGeometry, objects[j]->mBoundingGeometry, point, normal);

        cube->applyForce(Vec3f(0, 500, 0));
        cube->applyTorque(point, Vec3f(0, 500, 0));
        
        // handle collision...
      }
    }
  }



}
