#pragma once

#include "cinder/Vector.h"

using namespace ci;

//
//
//
// [t^3 t^2 t 1] [-1  3 -3  1  [p1
//                 3 -6  3  0   p2
//							  -3  0  3  0   p3
//                 1  4  1  0]  p4]
//

class VfBSpline
{
public:
	 
	static float calc1D (float p1, float p2, float p3, float p4, float t)
	{
		float cubicWeight1 = (     -p1 + 3.0f*p2 - 3.0f*p3 + p4)/6.0f;
		float cubicWeight2 = ( 3.0f*p1 - 6.0f*p2 + 3.0f*p3	   )/6.0f;
		float cubicWeight3 = (-3.0f*p1           + 3.0f*p3     )/6.0f;
		float cubicWeight4 = (      p1 + 4.0f*p2 +      p3		 )/6.0f;
		
		return cubicWeight4 + t * (cubicWeight3 + t * (cubicWeight2 + (t * cubicWeight1)));
	}
	
	static Vec3f calc3D (Vec3f p1, Vec3f p2, Vec3f p3, Vec3f p4, float t)
	{
		return Vec3f (calc1D (p1.x, p2.x, p3.x, p4.x, t),
		              calc1D (p1.y, p2.y, p3.y, p4.y, t),
		              calc1D (p1.z, p2.z, p3.z, p4.z, t));
	}
	
};
