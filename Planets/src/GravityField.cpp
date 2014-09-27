#include "GravityField.h"
#include "cinder/gl/gl.h"

#include "CollisionManager.h"

using namespace ci;

static const float G = 0.000002f;

GravityField::GravityField (const Vec3f& size, const Vec3i& nofElements)
: mSize(size),
  mNofElements (nofElements),
  mElementSize(Vec3f (size.x/(float)(nofElements.x-1), 
                      size.y/(float)(nofElements.y-1), 
                      size.z/(float)(nofElements.z-1)))
{
  /*
  mGravityPotential.resize (mNofElements.x);
  for (uint32_t x=0; x<mNofElements.x; x++)
  {
    mGravityPotential[x].resize (mNofElements.y);
    for (uint32_t y=0; y<mNofElements.y; y++)
    {
      mGravityPotential[x][y].resize (mNofElements.z);
    }
  }
  */

//  resetField ();

	try {
	  mShader = gl::GlslProg (loadFile ("../Media/Shaders/gravity_vert.glsl"), 
                            loadFile ("../Media/Shaders/gravity_frag.glsl"));/*,  
                            loadFile ("../Media/Shaders/gravity_geom.glsl"), 
                            GL_POINTS, 
                            GL_LINE_STRIP, 
                            2*(10 + 1)*(10 + 1));*/
	}	catch (gl::GlslProgCompileExc &exc) {
		std::cout << "Shader compile error: " << std::endl;
		std::cout << exc.what();
	}	catch (...) {
		std::cout << "Unable to load shader" << std::endl;
	}

  initMesh();
}

/*
void GravityField::resetField ()
{
  for (uint32_t x=0; x<mNofElements.x; x++)
  {
    for (uint32_t y=0; y<mNofElements.y; y++)
    {
      for (uint32_t z=0; z<mNofElements.z; z++)
      {
        mGravityPotential[x][y][z] = Vec3f (0, 0, 0);
      }
    }
  }
}
*/

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

/*
const Vec3f GravityField::getGravityPotentialForObject (const Vec3f& objectPositon)
{
  if (!Collisions::isPointWithinCube (objectPositon, mPosition, mSize))
    return Vec3f (0, 0, 0);

  // Calculate which gravity slot the object belongs to
  uint32_t x = (uint32_t)((objectPositon.x - mPosition.x) / mElementSize.x);
  uint32_t y = (uint32_t)((objectPositon.y - mPosition.y) / mElementSize.y);
  uint32_t z = (uint32_t)((objectPositon.z - mPosition.z) / mElementSize.z);

  return mGravityPotential[x][y][z];
}
*/

void GravityField::applyGravity ()
{
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

/*
  for (set<DynamicObject *>::iterator it = mDynamicObjects.begin(); it != mDynamicObjects.end(); it++)
  {
    DynamicObject *dynamicObject = *it;

    Vec3f force = dynamicObject->getMass () * getGravityPotentialForObject (dynamicObject->getPosition ());

    dynamicObject->applyForce (force);
  }
*/
}

/*
void GravityField::update ()
{
  resetField ();

  for (uint32_t x=0; x<mNofElements.x; x++)
  {
    for (uint32_t y=0; y<mNofElements.y; y++)
    {
      for (uint32_t z=0; z<mNofElements.z; z++)
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
*/

/*
float getMinMax (float f, float min, float max)
{
  if (f > max)
    return max;

  if (f < min)
    return min;

  return f;
}
*/

void GravityField::draw ()
{
  mShader.bind ();

  const uint32_t nofMasses = mGravityObjects.size ();

  mMasses.resize (nofMasses);
  mRadius.resize (nofMasses);
  mPositions.resize (nofMasses);

  list <PhysicsObject *>::iterator it = mGravityObjects.begin ();
  for (uint32_t i=0; i < nofMasses; i++)
  {
    PhysicsObject *po = *it;

    mMasses[i]     = po->getMass ();
    mRadius[i]     = po->getRadius ();
    mPositions[i]  = po->getPosition ();

    it++;
  }

  mShader.uniform ("massPositions", mPositions.data (),  nofMasses);
  mShader.uniform ("masses",        mMasses.data (),     nofMasses);
  mShader.uniform ("radius",        mRadius.data (),     nofMasses);
  mShader.uniform ("elementSize",   mElementSize);
  mShader.uniform ("nofMasses",(int)nofMasses);

  gl::draw (mVboMesh);

  mShader.unbind ();
}

uint32_t GravityField::getIndexForMeshPosition (uint32_t x, uint32_t y, uint32_t z)
{
  return z * mNofElements.y * mNofElements.x + y * mNofElements.x + x;
}

void GravityField::initMesh()
{
	std::vector<uint32_t> vboIndices;
	gl::VboMesh::Layout   vboLayout;
	std::vector<Vec3f>    vboVertices;

  float halfX = ((float)(mNofElements.x-1) * mElementSize.x)/2.0f;
  float halfY = ((float)(mNofElements.y-1) * mElementSize.y)/2.0f;
  float halfZ = ((float)(mNofElements.z-1) * mElementSize.z)/2.0f;

  // Create positions
  for (uint32_t z=0; z<mNofElements.z; z++)
  {
    for (uint32_t y=0; y<mNofElements.y; y++)
    {
      for (uint32_t x=0; x<mNofElements.x; x++)
      {
        Vec3f position = Vec3f ((float)x * mElementSize.x - halfX, 
                                (float)y * mElementSize.y - halfY, 
                                (float)z * mElementSize.z - halfZ);
  			vboVertices.push_back (position);
      }
    }
  }

  // Create indices
  bool xReveresed = false;
  bool yReveresed = false;

  for (uint32_t z=0; z<mNofElements.z; z++)
  {
    for (uint32_t y=0; y<mNofElements.y; y++)
    {
      uint32_t yPos = yReveresed ? mNofElements.y-1-y : y;
      for (uint32_t x=0; x<mNofElements.x; x++)
      {
        uint32_t xPos = xReveresed ? mNofElements.x-1-x : x;
	      vboIndices.push_back (getIndexForMeshPosition (xPos, yPos, z));
      }
      xReveresed = !xReveresed;
    }
    yReveresed = !yReveresed;
  }

  for (uint32_t z=0; z<mNofElements.z; z++)
  {
    for (uint32_t x=0; x<mNofElements.x; x++)
    {
      uint32_t xPos = xReveresed ? mNofElements.x-1-x : x;
      for (uint32_t y=0; y<mNofElements.y; y++)
      {
        uint32_t yPos = yReveresed ? mNofElements.y-1-y : y;
	      vboIndices.push_back (getIndexForMeshPosition (xPos, yPos, z));
      }
      yReveresed = !yReveresed;
    }
    xReveresed = !xReveresed;
  }

  // Vbo settings
	vboLayout.setStaticIndices ();
	vboLayout.setStaticPositions ();

  // Create Vbo
  mVboMesh = gl::VboMesh (vboVertices.size(), vboIndices.size(), vboLayout, GL_LINE_STRIP);
	mVboMesh.bufferIndices (vboIndices);
	mVboMesh.bufferPositions (vboVertices);
	mVboMesh.unbindBuffers ();

	// Clean up
	vboIndices.clear ();
	vboVertices.clear ();
}



/*
  glDisable (GL_TEXTURE_2D);

  for (uint32_t z=mNofElements.z/2-1; z<mNofElements.z/2+2; z++)
//  for (uint32_t z=0; z<mNofElements.z; z++)
  {
    cz = (float)z * mElementSize.z;

    for (uint32_t x=0; x<mNofElements.x; x++)
    {
      cx = (float)x * mElementSize.x;

      glBegin(GL_LINE_STRIP);

      for (uint32_t y=0; y<mNofElements.y; y++)
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
    for (uint32_t y=0; y<mNofElements.y; y++)
    {
      cy = (float)y * mElementSize.y;

      glBegin(GL_LINE_STRIP);

      for (uint32_t x=0; x<mNofElements.x; x++)
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
*/
