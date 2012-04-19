#include "cinder/gl/gl.h"
#include "PhysicsObject.h"
#include "BoundingBox.h"
#include "BoundingPlane.h"
#include "BoundingSphere.h"
#include "math.h"

using namespace ci;
using namespace std;

//Physics object using bounding box
PhysicsObject::PhysicsObject(float mass, Vec3f& cog, float boundingBoxWidth, float boundingBoxHeight, float boundingBoxLength)
{
  init(mass, cog);

  BoundingBox *boundingBox = new BoundingBox(boundingBoxWidth, boundingBoxHeight, boundingBoxLength, mPosition, mOrientation);
  mBoundingGeometry = (BoundingGeometry*) boundingBox;
};

//Physics object using bounding plane (init plane using 3 vertecies)
PhysicsObject::PhysicsObject(float mass, Vec3f& cog, float boundingPlaneWidth, float boundingPlaneHeight, Vec3f vertex1, Vec3f vertex2, Vec3f vertex3)
{
  init(mass, cog);


  //calc local coordinate basis vectors:
  Vec3f e1 = vertex2-vertex1;
  e1.normalize();
  Vec3f e2 = vertex3-vertex1;
  e2.normalize();
  Vec3f normal = e1.cross(e2);

  float offset = normal.dot(vertex1);
  mPosition = normal*offset;
  mOrientation.setColumn(0, e1);
  mOrientation.setColumn(1, e2);
  mOrientation.setColumn(2, normal);

  BoundingPlane *boundingPlane = new BoundingPlane(boundingPlaneWidth, boundingPlaneHeight, mPosition, mOrientation);
  mBoundingGeometry = (BoundingGeometry*) boundingPlane;
};

//Physics object using bounding plane (init plane using position of centre of plane and orientation matrix)
PhysicsObject::PhysicsObject(float mass, Vec3f& cog, float boundingPlaneWidth, float boundingPlaneHeight)
{
  init(mass, cog);

  BoundingPlane *boundingPlane = new BoundingPlane(boundingPlaneWidth, boundingPlaneHeight, mPosition, mOrientation);
  mBoundingGeometry = (BoundingGeometry*) boundingPlane;
};

//Physics object using bounding sphere
PhysicsObject::PhysicsObject(float mass, Vec3f& cog, float boundingSphereRadius)
{
  init(mass, cog);

  BoundingSphere *boundingSphere = new BoundingSphere(boundingSphereRadius, mPosition, mOrientation);
  mBoundingGeometry = (BoundingGeometry*) boundingSphere;
};

void PhysicsObject::init(float mass, Vec3f& cog)
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


PhysicsObject::~PhysicsObject()
{
  delete mBoundingGeometry;
}

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

  Orthogonalize (mOrientation);

  mOrientation.setColumn(3, Vec4f(0,        0,          0, 1));

  mBoundingGeometry->setPosition(mPosition);
  mBoundingGeometry->setOrientation(mOrientation);
}

void PhysicsObject::draw()
{
  mBoundingGeometry->draw();
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

const Matrix44f& PhysicsObject::getOrientation()
{
  return mOrientation;
}

Matrix44f PhysicsObject::getSkewMatrix(Vec4f vector)
{
  Matrix44f skewMatrix;
  skewMatrix = Matrix44f::identity();

  skewMatrix.setColumn(0, Vec4f(0,          vector.z,   -vector.y,  0));
  skewMatrix.setColumn(1, Vec4f(-vector.z,  0,          vector.x,   0));
  skewMatrix.setColumn(2, Vec4f(vector.y,   -vector.x,  0,          0));

  return skewMatrix;
}

void PhysicsObject::Orthogonalize(Matrix44f& matrix)
{
  const float treshold = 0.000001f;

  Matrix44f matrixClone;
  Vec4f     tempColumn;
  float determinant;
  
  determinant = matrix.determinant ();

  for (int i=0; i<1000 && abs(determinant - 1.0f) > treshold; i++)
  {
    matrixClone = matrix;

    matrix.transpose ();
    matrix.invert ();

    matrix = (matrix + matrixClone) / 2.0f;

    // normalize
    for(int j=0; j<4; j++)
    {
      tempColumn = matrix.getColumn(j);
      tempColumn.normalize();
      matrix.setColumn(j, tempColumn);
    }

    determinant = matrix.determinant ();
  }
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

Vec4f PhysicsObject::EulerForward(Vec4f f, Vec4f fPrim, float dt)
{
  Vec4f ans;
  ans = Vec3f(0, 0, 0);

  ans.x = f.x + fPrim.x * dt;
  ans.y = f.y + fPrim.y * dt;
  ans.z = f.z + fPrim.z * dt;
  ans.w = f.w + fPrim.w * dt;

  return ans;
}

Matrix44f PhysicsObject::EulerForward(Matrix44f f, Matrix44f fPrim, float dt)
{
  Matrix44f ans;
  return f + fPrim * dt;
}

