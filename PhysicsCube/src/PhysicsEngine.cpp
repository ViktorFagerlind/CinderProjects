#include "cinder/gl/gl.h"
#include "PhysicsEngine.h"
#include "BoundingGeometry.h"
#include "CollisionDetection.h"
#include "math.h"

using namespace ci;
using namespace std;

PhysicsEngine::PhysicsEngine()
{
  cube = new DynamicObject(1.0f, vec3(0, 0, 0), 70.0f, 70.0f, 70.0f);
  cube->setPosition(vec3(0, 500, 0));

  plane = new StaticObject(1.0f, vec3(0, 0, 0), 70.0f, 70.0f, vec3(0, 0, 100), vec3(100, 0, 0), vec3(0, 0, 0));

  sphere = new DynamicObject(1.0f, vec3(0, 0, 0), 35.0f);
  sphere->setPosition(vec3(100, 0, 0));

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

  //vec3 collisionPoint = getCollisionPoint();

 cube->applyForce(vec3(0, -50, 0));
 cube->applyTorque(vec3(1, 0, 0), vec3(0, -50, 0));


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

void PhysicsEngine::applyForce(PhysicsObject physicsObject, vec3 force)
{

}
  
void PhysicsEngine::applyTorque(PhysicsObject physicsObject, vec3 pointOfAttack, vec3 force)
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
        vec3 normal, point;

        CollisionDetection::getCollisionPoint (objects[i]->mBoundingGeometry, objects[j]->mBoundingGeometry, point, normal);

        
        vec3 bodyAVelocity = objects[i]->getPointVelocity(point);
        vec3 bodyBVelocity = objects[j]->getPointVelocity(point);

        vec3 APvector =  point - objects[i]->mState.mPosition;
        vec3 BPvector =  point - objects[j]->mState.mPosition;

        vec3 relativeVelocity = bodyAVelocity - bodyBVelocity;

        float bounceCoeff = 0.5;
        float nominator = -(1+bounceCoeff)*(relativeVelocity).dot(normal);
        float denominator1 = (1/objects[i]->mMass + 1/objects[j]->mMass)*normal.dot(normal);
        
        vec3 APnAP = APvector;
        APnAP       = APnAP.cross(normal);
        APnAP       = APnAP.cross(APvector);
        
        vec3 BPnBP = BPvector;
        BPnBP       = BPvector.cross(normal);
        BPnBP       = BPnBP.cross(BPvector);

        //float denominator2 = (objects[i]->getGlobalInertiaInverted()*APnAP + objects[j]->getGlobalInertiaInverted()*BPnBP).dot(normal);
        float denominator2 = (objects[j]->getGlobalInertiaInverted()*BPnBP).dot(normal);


        float collisionImpulse = nominator/(denominator1 + denominator2);
        
        objects[i]->setLinearMomentum(objects[i]->mState.mLinearMomentum + collisionImpulse*normal);
        
        vec3 AngularImpulse3D = APvector.cross(collisionImpulse*normal);
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
