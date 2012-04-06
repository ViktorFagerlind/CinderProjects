#include "GravityField.h"
#include "cinder/gl/gl.h"

#include "CollisionManager.h"

using namespace ci;

static const float G = 0.000002f;

GravityField::GravityField (const Vec3f& position, const Vec3f& size, const Vec3i& nofElements)
: mPosition (position),
  mSize(size),
  mNofElements (nofElements),
  mElementSize(Vec3f (size.x/(float)nofElements.x, size.y/(float)nofElements.y, size.z/(float)nofElements.z))
{
  mGravityPotential.resize (mNofElements.x);
  for (unsigned int x=0; x<mNofElements.x; x++)
  {
    mGravityPotential[x].resize (mNofElements.y);
    for (unsigned int y=0; y<mNofElements.y; y++)
    {
      mGravityPotential[x][y].resize (mNofElements.z);
    }
  }

  resetField ();
}

void GravityField::resetField ()
{
  for (unsigned int x=0; x<mNofElements.x; x++)
  {
    for (unsigned int y=0; y<mNofElements.y; y++)
    {
      for (unsigned int z=0; z<mNofElements.z; z++)
      {
        mGravityPotential[x][y][z] = Vec3f (0, 0, 0);
      }
    }
  }
}

void GravityField::addGravityObject (PhysicsObject *object)  
{
  mGravityObjects.push_back (object);
}

void GravityField::removeGravityObject (PhysicsObject *object)
{
  mGravityObjects.remove (object);
}

void GravityField::addDynamicObject (DynamicObject *object) 
{
  mDynamicObjects.insert (object);
}

void GravityField::removeDynamicObject (DynamicObject *object)
{
  set<DynamicObject *>::iterator it = mDynamicObjects.find (object);

  mDynamicObjects.erase (it);
}

bool GravityField::isStaticCollision (PhysicsObject *object)
{
  for (list<PhysicsObject *>::iterator it = mGravityObjects.begin(); it != mGravityObjects.end(); it++)
  {
    PhysicsObject *testObject = *it;

    // Dont test towards the object itself
    if (object == testObject)
      break;

    if (object->getPosition ().distance (testObject->getPosition ()) < object->getRadius () + testObject->getRadius ())
      return true;
  }

  return false;
}

bool GravityField::isDynamicCollision (PhysicsObject *object)
{
  for (set<DynamicObject *>::iterator it = mDynamicObjects.begin(); it != mDynamicObjects.end(); it++)
  {
    PhysicsObject *testObject = *it;

    // Dont test towards the object itself
    if (object == testObject)
      break;

    if (object->getPosition ().distance (testObject->getPosition ()) < object->getRadius () + testObject->getRadius ())
      return true;
  }

  return false;
}


bool GravityField::isCollision (PhysicsObject *object)
{
  return isStaticCollision (object) || isDynamicCollision (object);
}

Vec3f GravityField::calculateGravityForce  (const Vec3f& subjectPosition, 
                                            float subjectMass, 
                                            const Vec3f& objectPosition, 
                                            float objectMass)
{
  // F = G * m1 * m2 / r^2
  Vec3f distance  = objectPosition - subjectPosition;
  float forceSize = G * subjectMass * objectMass / distance.lengthSquared ();
  return forceSize * distance.normalized ();
}

const Vec3f GravityField::getGravityPotentialForObject (const Vec3f& objectPositon)
{
  if (!Collisions::isPointWithinCube (objectPositon, mPosition, mSize))
    return Vec3f (0, 0, 0);

  // Calculate which gravity slot the object belongs to
  unsigned int x = (unsigned int)((objectPositon.x - mPosition.x) / mElementSize.x);
  unsigned int y = (unsigned int)((objectPositon.y - mPosition.y) / mElementSize.y);
  unsigned int z = (unsigned int)((objectPositon.z - mPosition.z) / mElementSize.z);

  return mGravityPotential[x][y][z];
}

void GravityField::applyGravity ()
{
  /*
  for (set<DynamicObject *>::iterator it = mDynamicObjects.begin(); it != mDynamicObjects.end(); it++)
  {
    DynamicObject *dynamicObject = *it;

    for (list<PhysicsObject *>::iterator jt = mGravityObjects.begin(); jt != mGravityObjects.end(); jt++)
    {
      PhysicsObject *gravityObject = *jt;
  
      Vec3f force = calculateGravityForce (dynamicObject->getPosition (), dynamicObject->getMass (),
                                           gravityObject->getPosition (), gravityObject->getMass ());

      dynamicObject->applyForce (force);
    }
  }
  */

  for (set<DynamicObject *>::iterator it = mDynamicObjects.begin(); it != mDynamicObjects.end(); it++)
  {
    DynamicObject *dynamicObject = *it;

    Vec3f force = dynamicObject->getMass () * getGravityPotentialForObject (dynamicObject->getPosition ());

    dynamicObject->applyForce (force);
  }
}


void GravityField::update ()
{
  resetField ();

  for (unsigned int x=0; x<mNofElements.x; x++)
  {
    for (unsigned int y=0; y<mNofElements.y; y++)
    {
      for (unsigned int z=0; z<mNofElements.z; z++)
      {
        const Vec3f center = Vec3f ((float)x * mElementSize.x,
                                    (float)y * mElementSize.y,
                                    (float)z * mElementSize.z) + 0.5f * mElementSize + mPosition;

        for (list<PhysicsObject *>::iterator it = mGravityObjects.begin(); it != mGravityObjects.end(); it++)
        {
          PhysicsObject *gravityObject = *it;
          gravityObject->getPosition();

          mGravityPotential[x][y][z] += 
            calculateGravityForce (center, 1.0f, gravityObject->getPosition (), gravityObject->getMass ());
        }
      }
    }
  }
}

float getMinMax (float f, float min, float max)
{
  if (f > max)
    return max;

  if (f < min)
    return min;

  return f;
}

void GravityField::draw ()
{
  float cx, cy, cz;

  glDisable (GL_TEXTURE_2D);

  for (unsigned int z=mNofElements.z/2-1; z<mNofElements.z/2+2; z++)
//  for (unsigned int z=0; z<mNofElements.z; z++)
  {
    cz = (float)z * mElementSize.z;

    for (unsigned int x=0; x<mNofElements.x; x++)
    {
      cx = (float)x * mElementSize.x;

      glBegin(GL_LINE_STRIP);

      for (unsigned int y=0; y<mNofElements.y; y++)
      {
        cy = (float)y * mElementSize.y;

        const Vec3f center = Vec3f (cx, cy, cz) + 0.5f * mElementSize + mPosition;
        const Vec3f& potential  = mGravityPotential[x][y][z];
        Vec3f fieldDisplacement = 150.0f * potential;

        float f = potential.length ();

        fieldDisplacement.x = getMinMax (fieldDisplacement.x, -0.5f*mElementSize.x, 0.5f*mElementSize.x);
        fieldDisplacement.y = getMinMax (fieldDisplacement.y, -0.5f*mElementSize.y, 0.5f*mElementSize.y);
        fieldDisplacement.z = getMinMax (fieldDisplacement.z, -0.5f*mElementSize.z, 0.5f*mElementSize.z);

        glColor4f (0.5f, 0.5f, 1.0f, f * 10.0f);
        glVertex3f (center + fieldDisplacement);
      }

      glEnd();
    }

    // XXXXXXXXXX REMOVE XXXXXXXXXXXXX
    for (unsigned int y=0; y<mNofElements.y; y++)
    {
      cy = (float)y * mElementSize.y;

      glBegin(GL_LINE_STRIP);

      for (unsigned int x=0; x<mNofElements.x; x++)
      {
        cx = (float)x * mElementSize.x;

        const Vec3f center = Vec3f (cx, cy, cz) + 0.5f * mElementSize + mPosition;
        const Vec3f& potential  = mGravityPotential[x][y][z];
        Vec3f fieldDisplacement = 150.0f * potential;

        float f = potential.length ();

        fieldDisplacement.x = getMinMax (fieldDisplacement.x, -0.5f*mElementSize.x, 0.5f*mElementSize.x);
        fieldDisplacement.y = getMinMax (fieldDisplacement.y, -0.5f*mElementSize.y, 0.5f*mElementSize.y);
        fieldDisplacement.z = getMinMax (fieldDisplacement.z, -0.5f*mElementSize.z, 0.5f*mElementSize.z);

        glColor4f (0.5f, 0.5f, 1.0f, f * 10.0f);
        glVertex3f (center + fieldDisplacement);
      }

      glEnd();
    }
    // XXXXXXXXXXXXXXXXXXXXXXX



  }
}