#pragma once

#include "cinder/gl/gl.h"
#include "cinder/Vector.h"
#include "cinder/CinderMath.h"

using namespace ci;

class RoadBlock
{
public:
  RoadBlock(const Vec3f& center, const Vec3f& up, const Vec3f& left)
	{
    mCenter = center;
		mUp = up;
		mLeft   = left;
	}

  static uint32_t getIterations () {return 6;}

  void draw (uint32_t i, float whereWeight)
  {
    const float sliceAngle = 2.0f * (float) M_PI / (float) getIterations ();
    const float angle1 = sliceAngle * (float) i;
    const float angle2 = sliceAngle * (float) (i+1);

    const float width   = 3.0f * (whereWeight + 0.5f);
    const float height  = 8.0f * (whereWeight + 0.5f);

    Vec3f p1 = mCenter + width * mLeft * cos(angle1) + height * mUp * sin(angle1);
    Vec3f p2 = mCenter + width * mLeft * cos(angle2) + height * mUp * sin(angle2);

    glNormal3f ((p1-mCenter).normalized());
    glVertex3f (p1);

    glNormal3f ((p2-mCenter).normalized());
    glVertex3f (p2);
  }

public:
  Vec3f   mCenter;
  Vec3f   mUp;
  Vec3f   mLeft;
};

