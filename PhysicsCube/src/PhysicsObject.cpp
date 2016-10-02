#include "cinder/gl/gl.h"
#include "PhysicsObject.h"
#include "BoundingBox.h"
#include "BoundingPlane.h"
#include "BoundingSphere.h"
#include "math.h"

using namespace ci;
using namespace std;


State::State()
{
  mPosition     = vec3(0.0f, 0.0f, 0.0f);
  //mVelocity     = vec3(0.0f, 0.0f, 0.0f);
  mAngularMomentum = Vec4f(0.0f, 0.0f, 0.0f, 0.0f);
  //mAcceleration = vec3(0.0f, 0.0f, 0.0f);

  //mRotationVect       = vec3(0.0f, 0.0f, 0.0f);
  //mRotationSpeedVect  = vec3(0.0f, 0.0f, 0.0f);
  mOrientation        = Matrix44f::identity();
  mOrientationSpeed   = Matrix44f::identity();
};

//Physics object using bounding box
PhysicsObject::PhysicsObject(float mass, vec3& cog, float boundingBoxWidth, float boundingBoxHeight, float boundingBoxLength)
{
  init(mass, cog);

  BoundingBox *boundingBox = new BoundingBox(boundingBoxWidth, boundingBoxHeight, boundingBoxLength, mState.mPosition, mState.mOrientation);
  mBoundingGeometry = (BoundingGeometry*) boundingBox;
};

//Physics object using bounding plane (init plane using 3 vertecies)
PhysicsObject::PhysicsObject(float mass, vec3& cog, float boundingPlaneWidth, float boundingPlaneHeight, vec3 vertex1, vec3 vertex2, vec3 vertex3)
{
  init(mass, cog);


  //calc local coordinate basis vectors:
  vec3 e1 = vertex2-vertex1;
  e1.normalize();
  vec3 e2 = vertex3-vertex1;
  e2.normalize();
  vec3 normal = e1.cross(e2);

  float offset = normal.dot(vertex1);
  mState.mPosition = normal*offset;
  mState.mOrientation.setColumn(0, e1);
  mState.mOrientation.setColumn(1, e2);
  mState.mOrientation.setColumn(2, normal);

  BoundingPlane *boundingPlane = new BoundingPlane(boundingPlaneWidth, boundingPlaneHeight, mState.mPosition, mState.mOrientation);
  mBoundingGeometry = (BoundingGeometry*) boundingPlane;
};

//Physics object using bounding plane (init plane using position of centre of plane and orientation matrix)
PhysicsObject::PhysicsObject(float mass, vec3& cog, float boundingPlaneWidth, float boundingPlaneHeight)
{
  init(mass, cog);

  BoundingPlane *boundingPlane = new BoundingPlane(boundingPlaneWidth, boundingPlaneHeight, mState.mPosition, mState.mOrientation);
  mBoundingGeometry = (BoundingGeometry*) boundingPlane;
};

//Physics object using bounding sphere
PhysicsObject::PhysicsObject(float mass, vec3& cog, float boundingSphereRadius)
{
  init(mass, cog);

  BoundingSphere *boundingSphere = new BoundingSphere(boundingSphereRadius, mState.mPosition, mState.mOrientation);
  mBoundingGeometry = (BoundingGeometry*) boundingSphere;
};

void PhysicsObject::init(float mass, vec3& cog)
{
  mMass     = mass;
  mCoG      = cog;

  // Inertia definition for a cube.
  float tempSideLength = 70.0f;
  mLocalInertia = Matrix44f::identity();
  mLocalInertia.m00 = (mMass / 6.0f)*tempSideLength*tempSideLength;
  mLocalInertia.m11 = (mMass / 6.0f)*tempSideLength*tempSideLength;  
  mLocalInertia.m22 = (mMass / 6.0f)*tempSideLength*tempSideLength; 
  
  mLocalInertiaInverted = mLocalInertia.inverted();

  mInertia = mState.mOrientation*mLocalInertia*mState.mOrientation.transposed();
  mInertiaInverted = mState.mOrientation*mLocalInertiaInverted*mState.mOrientation.transposed();
};


PhysicsObject::~PhysicsObject()
{
  delete mBoundingGeometry;
}

void PhysicsObject::update(float dt)
{
 // mState.mVelocity = EulerForward(mState.mVelocity, mState.mAcceleration, dt);
  mState.mPosition = EulerForward(mState.mPosition, mState.mLinearMomentum/mMass, dt);

  Matrix44f tempSkewMatrix;
  tempSkewMatrix = getSkewMatrix(getRotationVelocity());

  mState.mOrientationSpeed = tempSkewMatrix * mState.mOrientation;

  Matrix44f orientationAccel = tempSkewMatrix * mState.mOrientationSpeed;
  mState.mOrientationSpeed = EulerForward(mState.mOrientationSpeed, orientationAccel, dt);
  mState.mOrientation      = EulerForward(mState.mOrientation, mState.mOrientationSpeed, dt);

  Orthogonalize (mState.mOrientation);
  
  // update collision geometry
  mBoundingGeometry->setPosition(mState.mPosition);
  mBoundingGeometry->setOrientation(mState.mOrientation);

  resetForce();
  resetTorque();
}

void PhysicsObject::draw()
{
  mBoundingGeometry->draw();
}

void PhysicsObject::setPosition(vec3 position)
{
  mState.mPosition = position;
  mBoundingGeometry->setPosition(position); //Endast för testning.
}

void PhysicsObject::setVelocity(vec3 velocity)
{
  setLinearMomentum(mMass * velocity);
}

void PhysicsObject::setLinearMomentum(vec3 linearMomentum)
{
  mState.mLinearMomentum = linearMomentum;
}

void PhysicsObject::setAngularMomentum(Vec4f angularMomentum)
{
  mState.mAngularMomentum = angularMomentum;
}

vec3 PhysicsObject::getPosition()
{
  return mState.mPosition;
}

vec3 PhysicsObject::getVelocity()
{
  return mState.mLinearMomentum / mMass;
}

vec3 PhysicsObject::getRotationVelocity()
{
  vec3 rotationVelolcity3D = vec3(0, 0, 0);
  Vec4f rotationVelolcity4D;
  rotationVelolcity4D = mState.mOrientation * mLocalInertiaInverted * mState.mOrientation.transposed() * mState.mAngularMomentum;
  rotationVelolcity3D[0] = rotationVelolcity4D[0];
  rotationVelolcity3D[1] = rotationVelolcity4D[1]; 
  rotationVelolcity3D[2] = rotationVelolcity4D[2];

  return rotationVelolcity3D;
}

vec3 PhysicsObject::getPointVelocity(vec3 point)
{
  vec3 lever = point - mState.mPosition;
  return getVelocity() + getRotationVelocity().cross(lever);
}

const Matrix44f& PhysicsObject::getOrientation()
{
  return mState.mOrientation;
}

Matrix44f PhysicsObject::getGlobalInertiaInverted()
{
  return mState.mOrientation*mLocalInertiaInverted*mState.mOrientation.transposed();
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
  matrix.setColumn(3, Vec4f(0,        0,          0, 1));
}

float PhysicsObject::EulerForward(float f, float fPrim, float dt)
{
  return f + fPrim * dt;
}

vec3 PhysicsObject::EulerForward(vec3 f, vec3 fPrim, float dt)
{
  return f + fPrim*dt;
}

Vec4f PhysicsObject::EulerForward(Vec4f f, Vec4f fPrim, float dt)
{
  return f + fPrim*dt;
}

Matrix44f PhysicsObject::EulerForward(Matrix44f f, Matrix44f fPrim, float dt)
{
  return f + fPrim * dt;
}



