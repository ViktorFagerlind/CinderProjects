#include "cinder/gl/gl.h"
#include "BoundingSphere.h"
#include "math.h"

using namespace ci;
using namespace std;

BoundingSphere::BoundingSphere(float radius, Vec3f position, Matrix44f orientation)
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
  gl::drawSphere(Vec3f(0, 0, 0), mRadius, 12);

  glPopMatrix();
}
