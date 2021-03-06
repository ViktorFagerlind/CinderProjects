#include "cinder/gl/gl.h"
#include "BoundingBox.h"
#include "math.h"

using namespace ci;
using namespace std;

BoundingBox::BoundingBox(float width, float height, float length, vec3 position, Matrix44f orientation)
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

void BoundingBox::getVertecies (vec3 *vertices) const
{
  vec3 localVertecies[8];

  float halfWidth = mWidth/2.0f;
  float halfHeight = mHeight/2.0f;
  float halfLength = mLength/2.0f;

  localVertecies[0] = vec3(-halfWidth, -halfHeight, halfLength);
  localVertecies[1] = vec3( halfWidth, -halfHeight, halfLength);
  localVertecies[2] = vec3( halfWidth,  halfHeight, halfLength);
  localVertecies[3] = vec3(-halfWidth,  halfHeight, halfLength);
  localVertecies[4] = vec3(-halfWidth, -halfHeight, -halfLength);
  localVertecies[5] = vec3(-halfWidth, halfHeight, -halfLength);
  localVertecies[6] = vec3( halfWidth,  halfHeight, -halfLength);
  localVertecies[7] = vec3(halfWidth,  -halfHeight, -halfLength);

  for(int i=0; i<8; i++)
  {
    vertices[i] = mPosition + mOrientation*localVertecies[i]; 
  }
}