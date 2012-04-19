#include "cinder/gl/gl.h"
#include "BoundingBox.h"
#include "math.h"

using namespace ci;
using namespace std;

BoundingBox::BoundingBox(float width, float height, float length, Vec3f position, Matrix44f orientation)
  : BoundingGeometry(eBox, position, orientation)
{
  mWidth  = width;
  mHeight = height;
  mLength = length;
};

void BoundingBox::update()
{

}
void BoundingBox::draw()
{
  gl::color(0.7f, 0.7f, 0.7f);

  glPushMatrix();
  glTranslatef(mPosition.x, mPosition.y, mPosition.z);
  gl::multModelView(mOrientation);

  // Draw cube
  float halfWidth = mWidth/2.0f;
  float halfHeight = mHeight/2.0f;
  float halfLength = mLength/2.0f;

  glBegin(GL_LINE_STRIP);
  glVertex3f (-halfWidth, -halfHeight, -halfLength);
  glVertex3f ( halfWidth, -halfHeight, -halfLength);
  glVertex3f ( halfWidth,  halfHeight, -halfLength);
  glVertex3f (-halfWidth,  halfHeight, -halfLength);
  glVertex3f (-halfWidth, -halfHeight, -halfLength);
  glEnd();

  glBegin(GL_LINE_STRIP);
  glVertex3f (-halfWidth, -halfHeight,  halfLength);
  glVertex3f ( halfWidth, -halfHeight,  halfLength);
  glVertex3f ( halfWidth,  halfHeight,  halfLength);
  glVertex3f (-halfWidth,  halfHeight,  halfLength);
  glVertex3f (-halfWidth, -halfHeight,  halfLength);
  glEnd();

  glBegin(GL_LINES);
  glVertex3f (-halfWidth, -halfHeight, -halfLength);
  glVertex3f (-halfWidth, -halfHeight,  halfLength);

  glVertex3f ( halfWidth, -halfHeight, -halfLength);
  glVertex3f ( halfWidth, -halfHeight,  halfLength);

  glVertex3f ( halfWidth,  halfHeight, -halfLength);
  glVertex3f ( halfWidth,  halfHeight,  halfLength);

  glVertex3f (-halfWidth,  halfHeight, -halfLength);
  glVertex3f (-halfWidth,  halfHeight,  halfLength);
  glEnd();

  glPopMatrix();
}
