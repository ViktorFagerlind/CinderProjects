#pragma once

#include "cinder/cairo/Cairo.h"
#include "cinder/Color.h"
using namespace ci;

#include <vector>

//----------------------------------------------------------------------------------------------------------------------

class Sprout 
{
public:
	Sprout (const vec2 &aOrigin);
	Sprout (const vec2 &aOrigin, int aLifespan, float aSpeed, float aAngle, float aAngleDelta, float aStartEllipseRadius, 
                float aEndEllipseRadius, ColorA aStartColor, ColorA aEndColor, float aScale);

  float getAgeLerp () {return 1.0f - mLifespan / (float)mTotalLifespan;}
  float getCurrentRadius () {return lerp (mStartEllipseRadius, mEndEllipseRadius, getAgeLerp ());}
  ColorA getCurrentColor () {return lerp (mStartColor, mEndColor, getAgeLerp ());}

	static void		setWindowSize (int aWindowWidth, int aWindowHeight) { sWindowWidth = aWindowWidth; sWindowHeight = aWindowHeight; }
	static float	randomHue ();

	virtual bool isGrowing ();

	virtual bool isAlive () = 0;

	virtual void update ();
	virtual void draw (cairo::Context ctx);

protected:
	static int sWindowWidth, sWindowHeight;

	int		  mLifespan, mTotalLifespan;
	vec2	  mOrigin, mPos;
	float	  mSpeed;

	float	  mAngle, mAngleDelta;
	float	  mStartEllipseRadius, mEndEllipseRadius;
	ColorA	mStartColor, mEndColor;
	float	  mScale;
};

//----------------------------------------------------------------------------------------------------------------------

class Branch : public Sprout 
{
public:
	Branch (const vec2 &aOrigin, float flowerHue);
	Branch (const vec2 &aOrigin, int aLifespan, float aSpeed, float aAngle, float aAngleDelta, float aChangeProb, float aFlowerProb,
		  float aStartEllipseRadius, float aEndEllipseRadius, ColorA aStartColor, ColorA aEndColor, ColorA aFlowerColor, float aScale, float aBranchProb);
  ~Branch ();

	static const int    MAX_BRANCHES;
	static const int    INITIAL_SPAWN;
	static const float  FLOWER_PROB;
	static const float  FLOWER_SIZE;
  static const float  BRANCH_LIFE;
  static const float  BRANCH_PROB;

	virtual void update();
	virtual void draw( cairo::Context ctx );

	virtual bool isGrowing ();
	virtual bool isAlive ();

	void createInitialBranch (const vec2 &aOrigin, float baseHue, ColorA flowerColor);
	void createBranch ();
	
protected:
	bool	mIsRoot;
	float	mChangeProb;
  float	mFlowerProb;
  float mBranchProb;

  ColorA	mFlowerColor;

	std::vector<Sprout*> mSproutes;
};

//----------------------------------------------------------------------------------------------------------------------

class Leaf : public Sprout 
{
public:
	Leaf (const vec2 &aOrigin, int aLifespan, float aSpeed, float aAngle, float aAngleDelta, float aStartEllipseRadius, 
        float aEndEllipseRadius, ColorA aStartColor, ColorA aEndColor, float aScale, int aLaunchDelay);

	virtual bool isGrowing();
	virtual bool isAlive ();

	virtual void update();

protected:
	int		  mLaunchDelay;
};

//----------------------------------------------------------------------------------------------------------------------
