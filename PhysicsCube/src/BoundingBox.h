#pragma once
#include "cinder/Vector.h"
#include "cinder/Matrix.h"
#include "BoundingGeometry.h"

using namespace ci;
using namespace std;

class BoundingBox : public BoundingGeometry
{
public:
  BoundingBox(float width, float height, float length, vec3 position, Matrix44f orientation);
	void update();
	void draw();

  void getVertecies (vec3 *vertices) const;

protected:
  float mWidth;
  float mHeight;
  float mLength;
};
