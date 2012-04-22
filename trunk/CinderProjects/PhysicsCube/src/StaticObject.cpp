#include "cinder/gl/gl.h"
#include "StaticObject.h"
#include "math.h"

using namespace ci;
using namespace std;

//Static object using bounding box
StaticObject::StaticObject(float mass, Vec3f& cog,  float boundingBoxWidth, float boundingBoxHeight, float boundingBoxLength)
  : PhysicsObject(mass, cog, boundingBoxWidth, boundingBoxHeight, boundingBoxLength)
{
  StaticObject::init(mass, cog);
};

//Static object using bounding plane (init plane using 3 vertecies)
StaticObject::StaticObject(float mass, Vec3f& cog,  float boundingPlaneWidth, float boundingPlaneHeight, Vec3f vertex1, Vec3f vertex2, Vec3f vertex3)
  : PhysicsObject(mass, cog, boundingPlaneWidth, boundingPlaneHeight, vertex1, vertex2, vertex3)
{
  StaticObject::init(mass, cog);
};


//Static object using bounding plane (init plane using position of centre of plane and orientation matrix)
StaticObject::StaticObject(float mass, Vec3f& cog,  float boundingPlaneWidth, float boundingPlaneHeight)
  : PhysicsObject(mass, cog, boundingPlaneWidth, boundingPlaneHeight)
{
  StaticObject::init(mass, cog);
};

//Static object using bounding sphere
StaticObject::StaticObject(float mass, Vec3f& cog,  float sphereRadius)
  : PhysicsObject(mass, cog, sphereRadius)
{
  StaticObject::init(mass, cog);
};


void StaticObject::init(float mass, Vec3f& cog)
{
  Vec4f tempRot = mState.mRotationSpeedVect; 
};


  
void StaticObject::update(float dt)
{
  PhysicsObject::update(dt);
}

void StaticObject::draw()
{
  PhysicsObject::draw();

  gl::color(0.3f, 0.3f, 1.0f);
  gl::drawVector(mState.mPosition, mState.mPosition + mState.mVelocity, 10.0f, 5.0f);

  gl::color(0.3f, 1.0f, 0.3f);
  gl::drawVector(mState.mPosition + mState.mVelocity, mState.mPosition + mState.mVelocity + mState.mAcceleration, 10.0f, 5.0f);

  gl::color(1.0f, 0.3f, 0.3f);
  gl::drawVector(mState.mPosition, mState.mPosition + (50.0f * mState.mRotationSpeedVect), 10.0f, 5.0f);
}
