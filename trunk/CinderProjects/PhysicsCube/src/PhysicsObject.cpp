#include "cinder/gl/gl.h"
#include "PhysicsObject.h"
#include "math.h"

using namespace ci;
using namespace std;

PhysicsObject::PhysicsObject(float mass, Vec3f& cog)
{
  mMass     = mass;
  mCoG      = cog;

  mPosition     = Vec3f(0.0f, 0.0f, 0.0f);
  mVelocity     = Vec3f(0.0f, 0.0f, 0.0f);
  mAcceleration = Vec3f(0.0f, 0.0f, 0.0f);

  mRotationVect       = Vec3f(0.0f, 0.0f, 0.0f);
  mRotationSpeedVect  = Vec3f(0.0f, 0.0f, 0.0f);  
  mOrientation        = Matrix44f::identity();
  mOrientationSpeed   = Matrix44f::identity();

};

void PhysicsObject::update()
{
  float dt = 0.0167f;

  mVelocity = EulerForward(mVelocity, mAcceleration, dt);
  mPosition = EulerForward(mPosition, mVelocity, dt);

  /* Vec3f xCol;
  Vec3f yCol;
  Vec3f zCol;
  xCol.x = mOrientation.m00;
  xCol.y = mOrientation.m10;
  xCol.z = mOrientation.m20;

  yCol.x = mOrientation.m01;
  yCol.y = mOrientation.m11;
  yCol.z = mOrientation.m21;

  zCol.x = mOrientation.m02;
  zCol.y = mOrientation.m12;
  zCol.z = mOrientation.m22;

  float angle = mRotationSpeedVect.length();
  mRotationSpeedVect.normalize();

  xCol.rotate(mRotationSpeedVect, angle);
  yCol.rotate(mRotationSpeedVect, angle);
  zCol.rotate(mRotationSpeedVect, angle);

  mOrientation.setColumn(0, xCol);
  mOrientation.setColumn(1, yCol);
  mOrientation.setColumn(2, zCol);
  */

  Matrix44f tempSkewMatrix;
  tempSkewMatrix = getSkewMatrix(mRotationSpeedVect);

  mOrientationSpeed = tempSkewMatrix * mOrientation;

  Matrix44f orientationAccel;
  orientationAccel = Matrix44f::identity();
  orientationAccel = tempSkewMatrix * tempSkewMatrix ;
  mOrientationSpeed = EulerForward(mOrientationSpeed, orientationAccel, dt);

  orientationAccel  = tempSkewMatrix * mOrientationSpeed;
  mOrientationSpeed = tempSkewMatrix * mOrientation;
  mOrientation      = EulerForward(mOrientation, mOrientationSpeed, dt);
  mOrientation      = Orthogonalize(mOrientation);
  mOrientation.setColumn(3, Vec4f(0,        0,          0, 1));

  //float angle = 0.0f;
  //float cosAlpha = cos(angle);
  //float sinAlpha = sin(angle);
  //mOrientation.setColumn(0, Vec4f(cosAlpha, sinAlpha,  0, 0));
  //mOrientation.setColumn(1, Vec4f(-sinAlpha, cosAlpha,   0, 0));
  //mOrientation.setColumn(2, Vec4f(0,        0,          1, 0));
}

void PhysicsObject::draw()
{
  gl::color(0.7f, 0.7f, 0.9f);

  glPushMatrix();
  glTranslatef(mPosition.x, mPosition.y, mPosition.z);
  //glRotatef(mAngle, 0.0f, 0.0f, 1.0f);
  gl::multModelView(mOrientation);

  glBegin(GL_QUADS);
  float halfSize = 100/2;
  glVertex3f (-halfSize, -halfSize, 0);
  glVertex3f (halfSize, -halfSize, 0);
  glVertex3f (halfSize, halfSize, 0);
  glVertex3f (-halfSize, halfSize, 0);
  glEnd();
  glPopMatrix();
  
}

void PhysicsObject::setPosition(Vec3f position)
{
  mPosition = position;
}

void PhysicsObject::setVelocity(Vec3f velocity)
{
  mVelocity = velocity;
}

void PhysicsObject::setRotationVect(Vec3f rotationVect)
{
  mRotationVect = rotationVect;
}

Vec3f PhysicsObject:: getPosition()
{
  return mPosition;
}

Matrix44f PhysicsObject::getSkewMatrix(Vec3f vector)
{

  Matrix44f skewMatrix;
  skewMatrix = Matrix44f::identity();

  skewMatrix.setColumn(0, Vec4f(0,          vector.z,   -vector.y,  0));
  skewMatrix.setColumn(1, Vec4f(-vector.z,  0,          vector.x,   0));
  skewMatrix.setColumn(2, Vec4f(vector.y,   -vector.x,  0,          0));

  return skewMatrix;
}

Matrix44f PhysicsObject::Orthogonalize(Matrix44f matrix)
{
  Matrix44f tempMatrix = matrix;
  Vec4f     tempColumn;
  float determinant;
  
  for(int i=0; i<100; i++)
  {

  }
  
  for(int i=0; i<4; i++)
  {
    tempColumn = matrix.getColumn(i);
    tempColumn.normalize();
    tempMatrix.setColumn(i, tempColumn);
  }

  return tempMatrix;
}

float PhysicsObject::EulerForward(float f, float fPrim, float dt)
{
  return f + fPrim * dt;
}

Vec3f PhysicsObject::EulerForward(Vec3f f, Vec3f fPrim, float dt)
{
  Vec3f ans;
  ans = Vec3f(0, 0, 0);

  ans.x = f.x + fPrim.x * dt;
  ans.y = f.y + fPrim.y * dt;
  ans.z = f.z + fPrim.z * dt;

  return ans;
}

Matrix44f PhysicsObject::EulerForward(Matrix44f f, Matrix44f fPrim, float dt)
{
  Matrix44f ans;
  return f + fPrim * dt;
}

