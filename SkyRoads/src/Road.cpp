#include "Road.h"

#include "BSpline.h"
#include "ParticleSystemManager.h"
#include "ParticleSystem.h"
#include "CommonModifier.h"
#include "ColorModifier.h"
#include "PointEmitter.h"

#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

const float Road::splineTimeAdd     = 0.1f;     // how many points per spline section
const float Road::speed             = 230.0f;
const float Road::gravity           = 0.98f;     // 1 = no gravity, 0 = infinite
const uint32_t Road::maxLength  = 100;      // how many points per spline section

void Road::setupParticles ()
{
  // Particle system
  ParticleSystem *particleSystem = new ParticleSystem("../Media/Images/smoke.png");
  
  mHeadEmitter = new AreaEmitter (4000,                  // maxNofParticles,
                                   vec3(0,0,0),         // position, 
  						                     20,                   // particlesPerFrame, 
  						                     15,                   // width
  						                     15,                   // height 
                                   15,                   // depth,
							                     5,                   // minParticleSize,
							                     10,                   // maxParticleSize,
							                     vec3 (0, 0, 0),      // baseVelocity,
							                     3.0f);                // randVelocity


  CommonModifier *commonModifier = new CommonModifier (1.0f, 1.0f, 5.0f);
  ColorModifier  *colorModifier  = new ColorModifier  (ColorAf(0.7f,  0.2f,  0.2f,  0.0f),  //startColor 
                                                       ColorAf(0.4f,  0.3f,  0.6f,  0.1f),  //middleColor
                                                       ColorAf(0.4f,  0.6f,  0.2f,  0.0f),  //endColor
                                                       0.8f);                               //float middleTime)
  particleSystem->addModifier (commonModifier);
  particleSystem->addModifier (colorModifier);
  particleSystem->addEmitter  (mHeadEmitter);

  ParticleSystemManager::getSingleton().addParticleSystem (particleSystem);
}

Road::Road ()
{
  // Init center points and normals
  mPreviousCenter = vec3::zero();
  mCurrentCenter  = vec3::zero();
  mNextCenter     = vec3::zero();
  mPreviousNormal = vec3(0, 1, 0);
    
  // Create initial steer points
  mSteerPoints = new vec3[4];
  for (int i=0; i<4; i++)
  {
    mSteerPoints[i] = vec3::zero ();
  }
    
  mSplineTime    = 0.0f;
    
  setupParticles ();

  // Let the road go for a while to avoid the boring beginning
  for (int i=0; i<20; i++)
    update();

  roadBlocks.clear ();
}
  
void Road::updateVectors (const vec3& center)
{
  vec3 previousToCurrent;
  vec3 nextToCurrent;
  vec3 tmp;

  mPreviousCenter  = mCurrentCenter;
  mCurrentCenter   = mNextCenter;
  mNextCenter      = center;

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
void Road::addRoadBlock (const vec3& center)
{
  vec3 normal;
    
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
  vec3 newSteerPoint; 
    
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
  vec3 nextPoint = VfBSpline::calc3D (mSteerPoints[0], mSteerPoints[1], mSteerPoints[2], mSteerPoints[3], mSplineTime);
    
  addRoadBlock (nextPoint);

  mHeadEmitter->setPosition (nextPoint);
}
	
void Road::draw ()
{
  uint32_t nofRoadBlocks = roadBlocks.size();

  if (nofRoadBlocks < 3)
    return;
  
  for (uint32_t i=0; i<RoadBlock::getIterations (); i++)
  {
    uint32_t currentRoadBlockIndex = 0;

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
	
const vec3& Road::getCurrentRoadEnd ()
{
	return mCurrentCenter;
}

const RoadBlock& Road::getFirstRoadBlock ()
{
  list<RoadBlock *>::iterator it = roadBlocks.begin();

  RoadBlock *rb = *it;

  return *rb;
}

