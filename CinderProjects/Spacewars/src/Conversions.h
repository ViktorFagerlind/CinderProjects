/*
 *  Utils.h
 *  BasicBox2D
 *
 *  Created by David Wicks on 6/7/10.
 *  Copyright 2010 David Wicks. All rights reserved.
 *
 */

#pragma once
#include "cinder/Vector.h"
#include "Box2D/Box2D.h"

using namespace ci;

class Conversions 
{
public: 
  static float getScaling() {static float scaling = 50.0f; return scaling;}
			
	static Vec3f fromPhysics3f (b2Vec2 fin)
	{
		return Vec3f(fin.x, fin.y, 0.f) * getScaling();
	}

	static Vec2f fromPhysics (b2Vec2 fin)
	{
		return Vec2f(fin.x, fin.y) * getScaling();
	}
			
	static b2Vec2 toPhysics (Vec2f fin)
	{
		return b2Vec2( fin.x/getScaling(), fin.y/getScaling() );
	}
			
	static float toPhysics (float fin)
	{
		return fin / getScaling();
	}
			
	static float fromPhysics (float fin)
	{
		return fin * getScaling();
	}
			
	static float radiansToDegrees (float rad)
	{
		return rad * 180.0f/(float)M_PI;
	}

  static Vec2f Vec3fTo2f (const Vec3f& v)
  {
    return Vec2f (v.x, v.y);
  }

  static Vec3f Vec2fTo3f (const Vec2f& v)
  {
    return Vec3f (v.x, v.y, 0.f);
  }
};
