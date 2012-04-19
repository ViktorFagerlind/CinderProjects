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
  	void init(float mass, Vec3f& cog);

public:
  DynamicObject(float mass, Vec3f& cog,  float boundingBoxWidth, float boundingBoxHeight, float boundingBoxLength);
  DynamicObject(float mass, Vec3f& cog,  float boundingPlaneWidth, float boundingPlaneHeight);
  DynamicObject(float mass, Vec3f& cog,  float boundingSphereRadius);
  void update();
	void draw();
  void applyForce(Vec3f force);
  void resetForce();
  void applyTorque(Vec3f pointOfAttack, Vec3f force); //point of attack and force in global reference frame
  void resetTorque();

protected:
  Matrix44f mLocalInertia;
  Matrix44f mLocalInertiaInverted;
  Matrix44f mInertia;
  Matrix44f mInertiaInverted;
  Vec4f mAngularMomentum; //Ev. ändra alla Vec3f till Vec4f. Verkar inte fungera så bra att muliplicera Matrix44f med Vect3f..
  Vec3f mForceSum;
  Vec3f mTorqueSum;
};
