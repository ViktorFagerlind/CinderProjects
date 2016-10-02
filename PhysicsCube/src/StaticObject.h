#pragma once
#include "cinder/Vector.h"
#include "cinder/Matrix.h"

#include "PhysicsObject.h"
#include "BoundingGeometry.h"

using namespace ci;
using namespace std;

class StaticObject : public PhysicsObject
{
private:
  	void init(float mass, vec3& cog);

public:
  StaticObject(float mass, vec3& cog,  float boundingBoxWidth, float boundingBoxHeight, float boundingBoxLength);
  StaticObject(float mass, vec3& cog,  float boundingPlaneWidth, float boundingPlaneHeight, vec3 vertex1, vec3 vertex2, vec3 vertex3); //Init plane using 3 vertecies
  StaticObject(float mass, vec3& cog,  float boundingPlaneWidth, float boundingPlaneHeight); //Init plane using position of centre of plane and orientation matrix
  StaticObject(float mass, vec3& cog,  float boundingSphereRadius);
  void update(float dt);
	void draw();
  
};
