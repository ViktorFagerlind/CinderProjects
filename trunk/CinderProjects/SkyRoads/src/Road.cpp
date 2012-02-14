#include "Road.h"

#include "BSpline.h"

#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

const float Road::splineTimeAdd     = 0.1f;     // how many points per spline section
const float Road::speed             = 230.0f;
const float Road::gravity           = 0.98f;     // 1 = no gravity, 0 = infinite
const unsigned int Road::maxLength  = 100;      // how many points per spline section


Road::Road ()
{
  // Init center points and normals
  mPreviousCenter = Vec3f::zero();
  mCurrentCenter  = Vec3f::zero();
  mNextCenter     = Vec3f::zero();
  mPreviousNormal = Vec3f(0, 1, 0);
    
  // Create initial steer points
  mSteerPoints = new Vec3f[4];
  for (int i=0; i<4; i++)
  {
    mSteerPoints[i] = Vec3f::zero();
  }
    
  mSplineTime    = 0.0f;
    
  // Let the road go for a while to avoid the boring beginning
  for (int i=0; i<20; i++)
    update();

  roadBlocks.clear();
}
  
void Road::updateVectors (const Vec3f& center)
{
  Vec3f previousToCurrent;
  Vec3f nextToCurrent;
  Vec3f tmp;

  tmp              = mPreviousCenter; 
  mPreviousCenter  = mCurrentCenter;
  mCurrentCenter   = mNextCenter;
  mNextCenter      = tmp;
  mNextCenter.set(center);

  previousToCurrent = mCurrentCenter - mPreviousCenter;
  nextToCurrent     = mCurrentCenter - mNextCenter;
  mCurrentNormal    = previousToCurrent + nextToCurrent;

  if (mCurrentNormal.length () < 0.0001f)
    mCurrentNormal = mPreviousNormal;
  mCurrentNormal.normalize ();
    
  if (mPreviousNormal.dot (mCurrentNormal) < 0.0f ) // Is the angle greater than 90 degrees
    mCurrentNormal = -mCurrentNormal;
    
  mCurrentLeft = previousToCurrent.cross (mCurrentNormal);
  mCurrentLeft.normalize ();

  mPreviousNormal = mCurrentNormal;
}

// Requires at least one previous block...
void Road::addRoadBlock (const Vec3f& center)
{
  Vec3f normal;
    
  updateVectors (center);
    
  RoadBlock *block = new RoadBlock (mCurrentCenter, mCurrentNormal, mCurrentLeft);
    
  roadBlocks.push_back (block);
    
  if (roadBlocks.size() > maxLength)
  {
    roadBlocks.pop_front (); // remove first element
  }
}
  
void Road::update()
{
  Vec3f newSteerPoint; 
    
  mSplineTime += splineTimeAdd;
    
  // new steering point needed
  if (mSplineTime > 1.0f)
  {
    mSplineTime = 0.0f;
      
    mSteerPoints[0] = mSteerPoints[1];
    mSteerPoints[1] = mSteerPoints[2];
    mSteerPoints[2] = mSteerPoints[3];
      
    newSteerPoint = mSteerPoints[2] - mSteerPoints[1]; // Last distance traveled
    newSteerPoint.x += Rand::randFloat (-200, 200);       // Add randomness
    newSteerPoint.y += Rand::randFloat (-200, 200);
    newSteerPoint.z += Rand::randFloat (-200, 200);
    newSteerPoint.normalize ();                                   
    newSteerPoint = newSteerPoint * speed;           // Same speed...
    newSteerPoint = newSteerPoint + mSteerPoints[2];  // Add to current
    newSteerPoint = newSteerPoint * gravity;         // Gravitate towards the center
      
    mSteerPoints[3] = newSteerPoint; 
  }
    
  // Calculate next point and add road block
  Vec3f nextPoint = VfBSpline::calc3D (mSteerPoints[0], mSteerPoints[1], mSteerPoints[2], mSteerPoints[3], mSplineTime);
    
  addRoadBlock (nextPoint);
}
	
void Road::draw ()
{
  unsigned int nofRoadBlocks = roadBlocks.size();

  if (nofRoadBlocks < 3)
    return;
  
  for (unsigned int i=0; i<RoadBlock::getIterations (); i++)
  {
    unsigned int currentRoadBlockIndex = 0;

    glBegin (GL_QUAD_STRIP);
    for (list<RoadBlock *>::iterator it = roadBlocks.begin(); it != roadBlocks.end(); it++)
    {
      RoadBlock *rb = *it;

      rb->draw (i, (float)currentRoadBlockIndex / (float)nofRoadBlocks);

      currentRoadBlockIndex++;
    }
    glEnd ();
  }
}
	
const Vec3f& Road::getCurrentRoadEnd ()
{
	return mCurrentCenter;
}

const RoadBlock& Road::getFirstRoadBlock ()
{
  list<RoadBlock *>::iterator it = roadBlocks.begin();

  RoadBlock *rb = *it;

  return *rb;
}

