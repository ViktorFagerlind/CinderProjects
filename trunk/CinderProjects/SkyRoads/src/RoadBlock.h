#pragma once

#include "cinder/gl/gl.h"
#include "cinder/Vector.h"

using namespace ci;

class RoadBlock
{
public:
  RoadBlock(const Vec3f& center, const Vec3f& normal, const Vec3f& left)
	{
    mCenter = center;
		mNormal = normal;
		mLeft   = left;
	}

  static uint32_t getIterations () {return 4;}

  void draw (uint32_t i, float whereWeight)
  {
    const Vec3f upAdd   = 7.0f  * mNormal * (whereWeight + 0.3f);
    const Vec3f leftAdd = 10.0f * mLeft * (whereWeight + 0.3f);

    Vec3f leftNormal = mLeft;
    leftNormal.normalize ();

    switch (i)
    {
      case 0:
        glNormal3fv (mNormal);
        glVertex3fv (mCenter + upAdd + leftAdd);
        glVertex3fv (mCenter + upAdd - leftAdd);
        break;
      case 1:
        glNormal3fv (-mNormal);
        glVertex3fv (mCenter - upAdd + leftAdd);
        glVertex3fv (mCenter - upAdd - leftAdd);
        break;
      case 2:
        glNormal3fv (leftNormal);
        glVertex3fv (mCenter + upAdd + leftAdd);
        glVertex3fv (mCenter - upAdd + leftAdd);
        break;
      case 3:
        glNormal3fv (-leftNormal);
        glVertex3fv (mCenter + upAdd - leftAdd);
        glVertex3fv (mCenter - upAdd - leftAdd);
        break;
    }
  }

public:
  Vec3f   mCenter;
  Vec3f   mNormal;
  Vec3f   mLeft;
};

