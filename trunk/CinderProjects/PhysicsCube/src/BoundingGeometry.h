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

  BoundingGeometry(Type shape, Vec3f position, Matrix44f orientation);
	virtual void update()=0;
	virtual void draw()=0;
  void setPosition(Vec3f position);
  Vec3f getPosition();
  void setOrientation(Matrix44f orientation);
  Matrix44f getOrientation();
  
  Type      mShape;

protected:
  Vec3f     mPosition;
  Matrix44f mOrientation;
};
