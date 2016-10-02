#pragma once
#include "cinder/Vector.h"
#include "cinder/Matrix.h"

#include "PhysicsObject.h"
#include "BoundingGeometry.h"

using namespace ci;
using namespace std;

class DynamicObject : public PhysicsObject
{
private:
  	void init(vec3& cog);

public:
  DynamicObject(float mass, vec3& cog,  float boundingBoxWidth, float boundingBoxHeight, float boundingBoxLength);
  DynamicObject(float mass, vec3& cog,  float boundingPlaneWidth, float boundingPlaneHeight);
  DynamicObject(float mass, vec3& cog,  float boundingSphereRadius);
  void update(float dt);
	void draw();
  void applyForce(vec3 force);
  void resetForce();
  void applyTorque(vec3 pointOfAttack, vec3 force); //point of attack and force in global reference frame
  void resetTorque();

  vec3 mForceSum;
  vec3 mTorqueSum;
};
