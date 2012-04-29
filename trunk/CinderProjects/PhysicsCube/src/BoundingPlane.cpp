#include "cinder/gl/gl.h"
#include "BoundingPlane.h"
#include "math.h"

using namespace ci;
using namespace std;

BoundingPlane::BoundingPlane(float width, float height, Vec3f position, Matrix44f orientation)
  : BoundingGeometry(ePlane, position, orientation)
{
  mWidth  = width;
  mHeigth = height;
};

void BoundingPlane::update()
{

}

Vec3f BoundingPlane::getNormal() const
{
  Vec3f tempNormal;
  Vec4f tempOrientationZAxis =  mOrientation.getColumn(2);

  tempNormal.x = tempOrientationZAxis.x;
  tempNormal.y = tempOrientationZAxis.y;
  tempNormal.z = tempOrientationZAxis.z;
  tempNormal.normalize();

  return tempNormal;
}

float BoundingPlane::getOffset() const
{
  Vec3f normal = getNormal();
  float offset = normal.dot(mPosition);
  return offset;
}

void BoundingPlane::draw()
{
  gl::color(0.7f, 0.7f, 0.7f);

  glPushMatrix();
  glTranslatef(mPosition.x, mPosition.y, mPosition.z);
  gl::multModelView(mOrientation);

  // Draw plane
  float halfWidth   = mWidth/2.0f;
  float halfHeigth  = mHeigth/2.0f;
  glBegin(GL_LINE_STRIP);
  glVertex3f (-halfWidth, -halfHeigth, 0);
  glVertex3f ( halfWidth, -halfHeigth, 0);
  glVertex3f ( halfWidth,  halfHeigth, 0);
  glVertex3f (-halfWidth,  halfHeigth, 0);
  glVertex3f (-halfWidth, -halfHeigth, 0);
  glEnd();
  glPopMatrix();

  //Draw normal
  gl::color(0.1f, 0.1f, 1.0f);
  gl::drawVector(mPosition, mPosition + 25*getNormal(), 5.0f, 2.5f);

  //Draw "offset"
  gl::color(0.8f, 0.8f, 0.1f);
  Vec3f offsetVec = getOffset()*getNormal();
  gl::drawVector(Vec3f(0, 0, 0), offsetVec, 5.0f, 2.5f);
}
