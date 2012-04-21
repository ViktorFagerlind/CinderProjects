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
  	void init(float mass, Vec3f& cog);

public:
  StaticObject(float mass, Vec3f& cog,  float boundingBoxWidth, float boundingBoxHeight, float boundingBoxLength);
  StaticObject(float mass, Vec3f& cog,  float boundingPlaneWidth, float boundingPlaneHeight, Vec3f vertex1, Vec3f vertex2, Vec3f vertex3); //Init plane using 3 vertecies
  StaticObject(float mass, Vec3f& cog,  float boundingPlaneWidth, float boundingPlaneHeight); //Init plane using position of centre of plane and orientation matrix
  StaticObject(float mass, Vec3f& cog,  float boundingSphereRadius);
  void update(float dt);
	void draw();
  
};
