#include "Road.h"

#include "BSpline.h"

#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

const float Road::splineTimeAdd     = 0.1f;     // how many points per spline section
const float Road::speed             = 200.0f;
const float Road::width             = 15.0f;
const float Road::gravity           = 0.9f;     // 1 = no gravity, 0 = infinite
const unsigned int Road::maxLength  = 400;      // how many points per spline section


Road::Road ()
{
  // Init center points and normals
  previousCenter = Vec3f::zero();
  currentCenter  = Vec3f::zero();
  nextCenter     = Vec3f::zero();
  previousNormal = Vec3f(0, 1, 0);
    
  // Create initial steer points
  steerPoints = new Vec3f[4];
  for (int i=0; i<4; i++)
  {
    steerPoints[i] = Vec3f::zero();
  }
    
  splineTime    = 0.0f;
    
  // Let the road go for a while to avoid the boring beginning
  for (int i=0; i<20; i++)
    update();

  roadBlocks.clear();
}
  
void Road::updateVectors (const Vec3f& center)
{
  Vec3f previousToCurrent;
  Vec3f currentToNext;
  Vec3f tmp;

  tmp             = previousCenter; 
  previousCenter  = currentCenter;
  currentCenter   = nextCenter;
  nextCenter      = tmp;
  nextCenter.set(center);
    
  previousToCurrent = currentCenter - previousCenter;
  currentToNext     = nextCenter - currentCenter;
  currentNormal     = previousToCurrent.cross (currentToNext);

  if (currentNormal.length () < 0.0001f)
    currentNormal = previousNormal;
  currentNormal.normalize ();
    
  if (previousNormal.dot (currentNormal) < 0.0f ) // Is the angle greater than 90 degrees
    currentNormal = -currentNormal;
    
  previousNormal.set(currentNormal);
}

// Requires at least one previous block...
void Road::addRoadBlock (const Vec3f& center)
{
  Vec3f normal;
    
  updateVectors (center);
    
  // Calculate normal
  Vec3f left  = currentCenter + width * currentNormal; // currentCenter + width * currentNormal
  Vec3f right = currentCenter - width * currentNormal; // currentCenter - width * currentNormal
    
  if (roadBlocks.size () > 0)
    normal = (right - roadBlocks.back()->mRight).cross (left - right);
  else
    normal = Vec3f::zero ();
    
  RoadBlock *block = new RoadBlock (left, right, normal);
    
  roadBlocks.push_back (block);
    
  if (roadBlocks.size() > maxLength)
  {
    roadBlocks.pop_front (); // remove first element
  }
}
  
void Road::update()
{
  Vec3f newSteerPoint; 
    
  splineTime += splineTimeAdd;
    
  // new steering point needed
  if (splineTime > 1.0f)
  {
    splineTime = 0.0f;
      
    steerPoints[0] = steerPoints[1];
    steerPoints[1] = steerPoints[2];
    steerPoints[2] = steerPoints[3];
      
    newSteerPoint = steerPoints[2] - steerPoints[1]; // Last distance traveled
    newSteerPoint.x += Rand::randFloat (-100, 100);       // Add randomness
    newSteerPoint.y += Rand::randFloat (-100, 100);
    newSteerPoint.z += Rand::randFloat (-100, 100);
    newSteerPoint.normalize ();                                   
    newSteerPoint = newSteerPoint * speed;           // Same speed...
    newSteerPoint = newSteerPoint + steerPoints[2];  // Add to current
    newSteerPoint = newSteerPoint * gravity;         // Gravitate towards the center
      
    steerPoints[3] = newSteerPoint; 
  }
    
  // Calculate next point and add road block
  Vec3f nextPoint = VfBSpline::calc3D (steerPoints[0], steerPoints[1], steerPoints[2], steerPoints[3], splineTime);
    
  addRoadBlock (nextPoint);
}
	
void Road::draw ()
{
  if (roadBlocks.size() < 3)
  {
    return;
  }
  
  glBegin (GL_QUAD_STRIP);
   
  for (list<RoadBlock *>::iterator it = roadBlocks.begin(); it != roadBlocks.end(); it++)
  {
    RoadBlock *rb = *it;

    glNormal3f (rb->mNormal.x, rb->mNormal.y, rb->mNormal.z);
    glVertex3f (rb->mLeft.x,  rb->mLeft.y,  rb->mLeft.z);
    glVertex3f (rb->mRight.x, rb->mRight.y, rb->mRight.z);
  }

  glEnd ();
}
	
const Vec3f& Road::getCurrentRoadEnd ()
{
	return currentCenter;
}

const RoadBlock& Road::getFirstRoadBlock ()
{
  list<RoadBlock *>::iterator it = roadBlocks.begin();

  RoadBlock *rb = *it;

  return *rb;
}

