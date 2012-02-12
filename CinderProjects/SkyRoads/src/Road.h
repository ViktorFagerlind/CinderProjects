#pragma once

#include "RoadBlock.h"

#include "cinder/Vector.h"
#include <list>

using std::list;

using namespace ci;


class Road
{
public:
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
  
  Vec3f  previousCenter;
  Vec3f  currentCenter;
  Vec3f  nextCenter;
  
  Vec3f  previousNormal;
  Vec3f  currentNormal;
  
  Vec3f *steerPoints;
  
  float  splineTime;
  
  // Constants
  static const float splineTimeAdd; // how many points per spline section
  static const float speed;
  static const float width;
  static const float gravity;       // 1 = no gravity, 0 = infinite
  static const unsigned int maxLength;     // how many points per spline section
};

