#pragma once
#include "cinder/Vector.h"
#include "cinder/Matrix.h"
#include "BoundingGeometry.h"

using namespace ci;
using namespace std;

class BoundingPlane : public BoundingGeometry
{
public:
  BoundingPlane(float width, float heigth, vec3 position, Matrix44f orientation);
	void update();
	void draw();
  vec3 getNormal() const;
  float getOffset() const;

protected:
  //plane equation: normal.*vector + offset = 0
  float mWidth;
  float mHeigth;
  vec3 mNormal;
  float mOffset;
};
