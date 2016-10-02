#pragma once
#include "cinder/Vector.h"
#include "cinder/Matrix.h"
#include "BoundingGeometry.h"

using namespace ci;
using namespace std;

class BoundingSphere : public BoundingGeometry
{
public:
  BoundingSphere(float radius, vec3 position, Matrix44f orientation);
	void update();
	void draw();

protected:
  float mRadius;
};
