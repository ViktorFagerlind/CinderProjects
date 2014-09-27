#include "cinder/gl/gl.h"
#include "BoundingGeometry.h"
#include "math.h"

using namespace ci;
using namespace std;

BoundingGeometry::BoundingGeometry(Type shape, Vec3f position, Matrix44f orientation)
{
  mShape        = shape;
  mPosition     = position;
  mOrientation  = orientation;
};

void BoundingGeometry::setPosition(Vec3f position)
{
  mPosition = position;
}

Vec3f BoundingGeometry::getPosition()
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