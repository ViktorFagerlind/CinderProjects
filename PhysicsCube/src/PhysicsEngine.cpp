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
  cube->setPosition(Vec3f(0, 500, 0));

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
 cube->applyTorque(Vec3f(1, 0, 0), Vec3f(0, -50, 0));


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

        
        Vec3f bodyAVelocity = objects[i]->getPointVelocity(point);
        Vec3f bodyBVelocity = objects[j]->getPointVelocity(point);

        Vec3f APvector =  point - objects[i]->mState.mPosition;
        Vec3f BPvector =  point - objects[j]->mState.mPosition;

        Vec3f relativeVelocity = bodyAVelocity - bodyBVelocity;

        float bounceCoeff = 0.5;
        float nominator = -(1+bounceCoeff)*(relativeVelocity).dot(normal);
        float denominator1 = (1/objects[i]->mMass + 1/objects[j]->mMass)*normal.dot(normal);
        
        Vec3f APnAP = APvector;
        APnAP       = APnAP.cross(normal);
        APnAP       = APnAP.cross(APvector);
        
        Vec3f BPnBP = BPvector;
        BPnBP       = BPvector.cross(normal);
        BPnBP       = BPnBP.cross(BPvector);

        //float denominator2 = (objects[i]->getGlobalInertiaInverted()*APnAP + objects[j]->getGlobalInertiaInverted()*BPnBP).dot(normal);
        float denominator2 = (objects[j]->getGlobalInertiaInverted()*BPnBP).dot(normal);


        float collisionImpulse = nominator/(denominator1 + denominator2);
        
        objects[i]->setLinearMomentum(objects[i]->mState.mLinearMomentum + collisionImpulse*normal);
        
        Vec3f AngularImpulse3D = APvector.cross(collisionImpulse*normal);
        Vec4f AngularImpulse4D;
        AngularImpulse4D[0] = AngularImpulse3D[0]; 
        AngularImpulse4D[1] = AngularImpulse3D[1];
        AngularImpulse4D[2] = AngularImpulse3D[2];
        AngularImpulse4D[3] = 0.0f;

        Vec4f tempAngularMomentum = objects[i]->mState.mAngularMomentum+AngularImpulse4D;
        objects[i]->setAngularMomentum(tempAngularMomentum);

        
        // handle collision...
      }
    }
  }



}
