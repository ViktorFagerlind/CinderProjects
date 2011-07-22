#pragma once

#include "cinder/cairo/Cairo.h"
#include "cinder/Color.h"
using namespace ci;

#include <vector>

//----------------------------------------------------------------------------------------------------------------------

class Sprout 
{
public:
	Sprout (const Vec2f &aOrigin);
	Sprout (const Vec2f &aOrigin, int aLifespan, float aSpeed, float aAngle, float aAngleDelta, float aStartEllipseRadius, 
                float aEndEllipseRadius, ColorA aStartColor, ColorA aEndColor, float aScale);

	static void		setWindowSize (int aWindowWidth, int aWindowHeight) { sWindowWidth = aWindowWidth; sWindowHeight = aWindowHeight; }
	static float	randomHue ();

	virtual bool isGrowing ();

	virtual bool isAlive () = 0;

	virtual void update ();
	virtual void draw (cairo::Context ctx);

protected:
	static int sWindowWidth, sWindowHeight;

	int		  mLifespan, mTotalLifespan;
	Vec2f	  mOrigin, mPos;
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
	Branch (const Vec2f &aOrigin, float flowerHue);
	Branch (const Vec2f &aOrigin, int aLifespan, float aSpeed, float aAngle, float aAngleDelta, float aChangeProb, float aFlowerProb,
		  float aStartEllipseRadius, float aEndEllipseRadius, ColorA aStartColor, ColorA aEndColor, ColorA aFlowerColor, float aScale);
  ~Branch ();

	static const int    MAX_BRANCHES;
	static const int    INITIAL_SPAWN;
	static const float  FLOWER_PROB;
	static const float  FLOWER_SIZE;


	virtual void update();
	virtual void draw( cairo::Context ctx );

	virtual bool isGrowing ();
	virtual bool isAlive ();

	void createBranch( const Vec2f &aOrigin, float baseHue, ColorA flowerColor);
	
protected:
	bool	mIsRoot;
	float	mChangeProb;
  float	mFlowerProb;

  ColorA	mFlowerColor;

	std::vector<Sprout*> mSproutes;
};

//----------------------------------------------------------------------------------------------------------------------

class Leaf : public Sprout 
{
public:
	Leaf (const Vec2f &aOrigin, int aLifespan, float aSpeed, float aAngle, float aAngleDelta, float aStartEllipseRadius, 
        float aEndEllipseRadius, ColorA aStartColor, ColorA aEndColor, float aScale, int aLaunchDelay);

	virtual bool isGrowing();
	virtual bool isAlive ();

	virtual void update();

protected:
	int		  mLaunchDelay;
};

//----------------------------------------------------------------------------------------------------------------------
