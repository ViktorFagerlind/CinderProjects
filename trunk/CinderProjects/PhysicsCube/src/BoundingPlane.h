#pragma once
#include "cinder/Vector.h"
#include "cinder/Matrix.h"
#include "BoundingGeometry.h"

using namespace ci;
using namespace std;

class BoundingPlane : public BoundingGeometry
{
public:
  BoundingPlane(float width, float heigth, Vec3f position, Matrix44f orientation);
	void update();
	void draw();
  Vec3f getNormal();
  float getOffset();

protected:
  //plane equation: normal.*vector + offset = 0
  float mWidth;
  float mHeigth;
  Vec3f mNormal;
  float mOffset;
};
