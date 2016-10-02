#include "cinder/gl/gl.h"
#include "BoundingGeometry.h"
#include "math.h"

using namespace ci;
using namespace std;

BoundingGeometry::BoundingGeometry(Type shape, vec3 position, Matrix44f orientation)
{
  mShape        = shape;
  mPosition     = position;
  mOrientation  = orientation;
};

void BoundingGeometry::setPosition(vec3 position)
{
  mPosition = position;
}

vec3 BoundingGeometry::getPosition()
{
  return mPosition;
}

void BoundingGeometry::setOrientation(Matrix44f orientation)
{
  mOrientation = orientation;
}

Matrix44f BoundingGeometry::getOrientation()
{
  return mOrientation;
}