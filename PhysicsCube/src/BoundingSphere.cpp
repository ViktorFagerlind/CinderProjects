#include "cinder/gl/gl.h"
#include "BoundingSphere.h"
#include "math.h"

using namespace ci;
using namespace std;

BoundingSphere::BoundingSphere(float radius, vec3 position, Matrix44f orientation)
  : BoundingGeometry(eSphere, position, orientation)
{
  mRadius = radius;
};

void BoundingSphere::update()
{

}
void BoundingSphere::draw()
{
  gl::color(0.7f, 0.7f, 0.7f);

  glPushMatrix();
  glTranslatef(mPosition.x, mPosition.y, mPosition.z);
  gl::multModelView(mOrientation);

  // Draw sphere
  gl::drawSphere(vec3(0, 0, 0), mRadius, 12);

  glPopMatrix();
}
