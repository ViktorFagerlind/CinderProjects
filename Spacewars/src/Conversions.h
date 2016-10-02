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
			
	static vec3 fromPhysics3f (b2Vec2 fin)
	{
		return vec3(fin.x, fin.y, 0.f) * getScaling();
	}

	static vec2 fromPhysics (b2Vec2 fin)
	{
		return vec2(fin.x, fin.y) * getScaling();
	}
			
	static b2Vec2 toPhysics (vec2 fin)
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

  static vec2 vec3To2f (const vec3& v)
  {
    return vec2 (v.x, v.y);
  }

  static vec3 vec2To3f (const vec2& v)
  {
    return vec3 (v.x, v.y, 0.f);
  }
};
