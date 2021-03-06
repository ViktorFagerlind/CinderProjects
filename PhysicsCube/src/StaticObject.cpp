#include "cinder/gl/gl.h"
#include "StaticObject.h"
#include "math.h"

using namespace ci;
using namespace std;

//Static object using bounding box
StaticObject::StaticObject(float mass, vec3& cog,  float boundingBoxWidth, float boundingBoxHeight, float boundingBoxLength)
  : PhysicsObject(mass, cog, boundingBoxWidth, boundingBoxHeight, boundingBoxLength)
{
  StaticObject::init(mass, cog);
};

//Static object using bounding plane (init plane using 3 vertecies)
StaticObject::StaticObject(float mass, vec3& cog,  float boundingPlaneWidth, float boundingPlaneHeight, vec3 vertex1, vec3 vertex2, vec3 vertex3)
  : PhysicsObject(mass, cog, boundingPlaneWidth, boundingPlaneHeight, vertex1, vertex2, vertex3)
{
  StaticObject::init(mass, cog);
};


//Static object using bounding plane (init plane using position of centre of plane and orientation matrix)
StaticObject::StaticObject(float mass, vec3& cog,  float boundingPlaneWidth, float boundingPlaneHeight)
  : PhysicsObject(mass, cog, boundingPlaneWidth, boundingPlaneHeight)
{
  StaticObject::init(mass, cog);
};

//Static object using bounding sphere
StaticObject::StaticObject(float mass, vec3& cog,  float sphereRadius)
  : PhysicsObject(mass, cog, sphereRadius)
{
  StaticObject::init(mass, cog);
};


void StaticObject::init(float mass, vec3& cog)
{
  mState.mLinearMomentum = vec3(0, 0, 0);
  mState.mAngularMomentum = Vec4f(0, 0, 0, 0);
};


  
void StaticObject::update(float dt)
{
  PhysicsObject::update(dt);
}

void StaticObject::draw()
{
  PhysicsObject::draw();

  gl::color(0.3f, 0.3f, 1.0f);
  gl::drawVector(mState.mPosition, mState.mPosition + getVelocity(), 10.0f, 5.0f);

 // gl::color(0.3f, 1.0f, 0.3f);
 // gl::drawVector(mState.mPosition + getVelocity(), mState.mPosition + getVelocity() + mState.mAcceleration, 10.0f, 5.0f);

  gl::color(1.0f, 0.3f, 0.3f);
  gl::drawVector(mState.mPosition, mState.mPosition + (50.0f * getVelocity()), 10.0f, 5.0f);
}
