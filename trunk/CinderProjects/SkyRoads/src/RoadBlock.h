#pragma once

#include "cinder/Vector.h"

using namespace ci;

class RoadBlock
{
public:
  RoadBlock(const Vec3f& left, const Vec3f& right, const Vec3f& normal)
	{
    mLeft   = left;
		mRight  = right;
		mNormal = normal;
	}

public:
  Vec3f   mLeft;
  Vec3f   mRight;
  Vec3f   mNormal;
};

