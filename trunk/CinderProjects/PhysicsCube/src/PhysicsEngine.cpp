#include "cinder/gl/gl.h"
#include "PhysicsEngine.h"
#include "BoundingGeometry.h"
#include "math.h"

using namespace ci;
using namespace std;

PhysicsEngine::PhysicsEngine()
{
  cube = new DynamicObject(1.0f, Vec3f(0, 0, 0), 70.0f, 70.0f, 70.0f);
  cube->setPosition(Vec3f(300, 0, 0));

  plane = new StaticObject(1.0f, Vec3f(0, 0, 0), 70.0f, 70.0f, Vec3f(0, 0, 100), Vec3f(100, 0, 0), Vec3f(0, 100, 0));

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

  // 2) If valid, update positions and orientations

  /* 2) Else: 1) determine collision time
          2) determine collision response (impulse)
          3) apply impulse
          4) update position and do 1)-3) until no collision.
  */

  Vec3f collisionPoint = getCollisionPoint();


  if (cube->getPosition().y > 300)
  {
    cube->applyForce(Vec3f(0, -50, 0));
    cube->applyTorque(Vec3f(1, 0, 0), Vec3f(0.0, 10.0, 0.0));
  }
  else
  {
    cube->applyForce(Vec3f(0, 50, 0));
  }

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

Vec3f PhysicsEngine::getCollisionPoint()
{
  Vec3f *collisionPoints;
  collisionPoints = new Vec3f[8];
  collisionPoints = cube->mBoundingGeometry->getVertecies();
  return collisionPoints[1];
}