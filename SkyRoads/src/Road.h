#pragma once

#include "RoadBlock.h"

#include "AreaEmitter.h"

#include "cinder/Vector.h"
#include <list>

using std::list;

using namespace ci;


class Road
{
public:
  void setupParticles ();

  Road ();
  
  void updateVectors (const vec3& center);

  // Requires at least one previous block...
  void addRoadBlock (const vec3& center);
  
  void update ();
	
	void draw ();
	
	const vec3& getCurrentRoadEnd ();

	const RoadBlock& getFirstRoadBlock ();

private:
  list<RoadBlock *> roadBlocks;
  
  vec3  mPreviousCenter;
  vec3  mCurrentCenter;
  vec3  mNextCenter;
  
  vec3  mPreviousNormal;
  vec3  mCurrentNormal;

  vec3  mCurrentLeft;
  
  vec3 *mSteerPoints;
  
  float  mSplineTime;
  
  // Constants
  static const float splineTimeAdd; // how many points per spline section
  static const float speed;
  static const float width;
  static const float gravity;       // 1 = no gravity, 0 = infinite
  static const uint32_t maxLength;     // how many points per spline section

  AreaEmitter *mHeadEmitter;
};

