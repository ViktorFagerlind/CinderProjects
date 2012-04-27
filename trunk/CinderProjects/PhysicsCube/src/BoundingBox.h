#pragma once
#include "cinder/Vector.h"
#include "cinder/Matrix.h"
#include "BoundingGeometry.h"

using namespace ci;
using namespace std;

class BoundingBox : public BoundingGeometry
{
public:
  BoundingBox(float width, float height, float length, Vec3f position, Matrix44f orientation);
	void update();
	void draw();
  Vec3f *getVertecies();

protected:
  float mWidth;
  float mHeight;
  float mLength;
};
