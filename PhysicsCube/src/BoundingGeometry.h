#pragma once
#include "cinder/Vector.h"
#include "cinder/Matrix.h"

using namespace ci;
using namespace std;

class BoundingGeometry
{
public:
  enum Type
  {
    eBox, eSphere, ePlane 
  };

  BoundingGeometry(Type shape, vec3 position, Matrix44f orientation);
	virtual void update()=0;
	virtual void draw()=0;
  void setPosition(vec3 position);
  vec3 getPosition();
  void setOrientation(Matrix44f orientation);
  Matrix44f getOrientation();
  
  Type      mShape;

protected:
  vec3     mPosition;
  Matrix44f mOrientation;
};
