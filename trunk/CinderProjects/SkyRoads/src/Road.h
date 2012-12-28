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
  
  void updateVectors (const Vec3f& center);

  // Requires at least one previous block...
  void addRoadBlock (const Vec3f& center);
  
  void update ();
	
	void draw ();
	
	const Vec3f& getCurrentRoadEnd ();

	const RoadBlock& getFirstRoadBlock ();

private:
  list<RoadBlock *> roadBlocks;
  
  Vec3f  mPreviousCenter;
  Vec3f  mCurrentCenter;
  Vec3f  mNextCenter;
  
  Vec3f  mPreviousNormal;
  Vec3f  mCurrentNormal;

  Vec3f  mCurrentLeft;
  
  Vec3f *mSteerPoints;
  
  float  mSplineTime;
  
  // Constants
  static const float splineTimeAdd; // how many points per spline section
  static const float speed;
  static const float width;
  static const float gravity;       // 1 = no gravity, 0 = infinite
  static const uint32_t maxLength;     // how many points per spline section

  AreaEmitter *mHeadEmitter;
};

