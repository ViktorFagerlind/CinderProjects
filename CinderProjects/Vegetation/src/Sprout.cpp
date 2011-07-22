#include "Sprout.h"

#include "cinder/Rand.h"

using std::vector;

int Sprout::sWindowWidth, Sprout::sWindowHeight;

const int    Branch::MAX_BRANCHES  = 250;
const int    Branch::INITIAL_SPAWN = 20;
const float  Branch::FLOWER_PROB   = 0.08f;
const float  Branch::FLOWER_SIZE   = 0.4f;

//----------------------------------------------------------------------------------------------------------------------

Sprout::Sprout (const Vec2f &aOrigin)
: mOrigin (aOrigin)
{
}

//----------------------------------------------------------------------------------------------------------------------

Branch::Branch( const Vec2f &aOrigin, float flowerHue)
: Sprout (aOrigin)
{
	mIsRoot = true;
	float baseHue = 0.33333f;
	
	float sat = 0.5 + Rand::randFloat( 0.5f );
	float brightness = 1.0;
	float opacity = 0.8 + Rand::randFloat( 0.2f );

	ColorA flowerColor( CM_HSV, flowerHue, sat, brightness, opacity );

	for( int i = 0; i < INITIAL_SPAWN; i++ ) {
		createBranch (mOrigin, baseHue, flowerColor);
	}
}

//----------------------------------------------------------------------------------------------------------------------

Sprout::Sprout (const Vec2f &aOrigin, int aLifespan, float aSpeed, float aAngle, float aAngleDelta, float aStartEllipseRadius, 
                float aEndEllipseRadius, ColorA aStartColor, ColorA aEndColor, float aScale)
	: mOrigin( aOrigin ), mPos( aOrigin )
{ 
	mLifespan = (int)(aLifespan * aScale);
	mTotalLifespan = (int)(aLifespan * aScale);
	mAngle = aAngle;
	mSpeed = aSpeed;
	mAngleDelta = aAngleDelta;
	mStartEllipseRadius = aStartEllipseRadius * aScale;
	mEndEllipseRadius = aEndEllipseRadius * aScale;
	mStartColor = aStartColor;
	mEndColor = aEndColor;
	mScale = aScale;
}

//----------------------------------------------------------------------------------------------------------------------

Branch::Branch (const Vec2f &aOrigin, int aLifespan, float aSpeed, float aAngle, float aAngleDelta, float aChangeProb, 
                float aFlowerProb, float aStartEllipseRadius, float aEndEllipseRadius, ColorA aStartColor, 
                ColorA aEndColor, ColorA aFlowerColor, float aScale)
: Sprout (aOrigin, aLifespan, aSpeed, aAngle, aAngleDelta, aStartEllipseRadius, aEndEllipseRadius, 
          aStartColor, aEndColor, aScale)
{ 
  mIsRoot = false;
	mChangeProb = aChangeProb;
	mFlowerProb = aFlowerProb;
	mFlowerColor = aFlowerColor;
}

//----------------------------------------------------------------------------------------------------------------------
Leaf::Leaf (const Vec2f &aOrigin, int aLifespan, float aSpeed, float aAngle, float aAngleDelta, float aStartEllipseRadius, 
            float aEndEllipseRadius, ColorA aStartColor, ColorA aEndColor, float aScale, int aLaunchDelay)
: Sprout (aOrigin, aLifespan, aSpeed, aAngle, aAngleDelta, aStartEllipseRadius, aEndEllipseRadius, 
          aStartColor, aEndColor, aScale)
{
  mLaunchDelay = aLaunchDelay;
}

//----------------------------------------------------------------------------------------------------------------------

Branch::~Branch()
{
	for( vector<Sprout*>::iterator it = mSproutes.begin(); it != mSproutes.end(); it++ )
		delete *it;
}

//----------------------------------------------------------------------------------------------------------------------

bool Sprout::isGrowing()
{
  return mLifespan > 0;
}

//----------------------------------------------------------------------------------------------------------------------

bool Branch::isGrowing()
{
  return Sprout::isGrowing() && !mIsRoot;
}

//----------------------------------------------------------------------------------------------------------------------

bool Leaf::isGrowing()
{
  return Sprout::isGrowing() && mLaunchDelay <= 0;
}

//----------------------------------------------------------------------------------------------------------------------

bool Branch::isAlive()
{
  if (!mIsRoot)
    return Sprout::isGrowing();

	for (vector<Sprout*>::iterator it = mSproutes.begin(); it != mSproutes.end(); it++)
  {	
    Sprout *sprout = *it;

    if (sprout->isAlive())
      return true;
  }
}

//----------------------------------------------------------------------------------------------------------------------

bool Leaf::isAlive()
{
  return mLaunchDelay > 0 || Sprout::isGrowing();
}

//----------------------------------------------------------------------------------------------------------------------

void Sprout::update()
{ 
	if (!isGrowing()) 
    return;

	mPos += Vec2f( cos( mAngle ), sin( mAngle ) ) * ( mSpeed * 2.0f * mScale );
	if( ! Area( 0, 0, sWindowWidth, sWindowHeight ).contains( mPos ) )
		mLifespan = 0;

	mAngle += mAngleDelta;
	mLifespan--;
}

//----------------------------------------------------------------------------------------------------------------------

void Branch::update()
{ 
	if (isGrowing()) 
  {
		if( Rand::randFloat() < mChangeProb )
			mAngleDelta = -mAngleDelta;

		float ageLerp = 1.0 - mLifespan / (float)mTotalLifespan;
		if( Rand::randFloat() < mFlowerProb * ( 1.0 - ageLerp ) ) 
    {
			if( mSproutes.size() < MAX_BRANCHES ) 
      {
        int launchDelay = Rand::randInt (40, 70);
        for (int i=0; i<4; i++)
        {
				  // Create blossom
          Leaf* newLeaf = new Leaf (mPos,                                                   /* aOrigin */
                                    20.0f,                                                  /* aLifespan */ 
                                    mSpeed * 0.7 * FLOWER_SIZE,                                   /* aSpeed */
                                    mAngle + 0.1f + (float)i * 3.14/2,                      /* aAngle */
                                    mAngleDelta*1.2f,                                       /* aAngleDelta */
                                    mEndEllipseRadius,                                      /* start radius */
	                                  15.0f*mScale*FLOWER_SIZE,                               /* aEndEllipseRadius */
                                    lerp( mFlowerColor, ColorA( 0, 0, 0, 1.0f ), 0.5f ),    /* aStartColor */
                                    mFlowerColor,                                           /* aEndColor */
                                    mScale,                                                 /* aScale */
                                    launchDelay);                                           /* aLaunchDelay */

          mSproutes.push_back (newLeaf);
        }
			}
		}
	}
  
  Sprout::update();

	for (vector<Sprout*>::iterator it = mSproutes.begin(); it != mSproutes.end(); it++)
  {	
    Sprout *sprout = *it;
    sprout->update();
  }
}

//----------------------------------------------------------------------------------------------------------------------

void Leaf::update()
{ 
	mLaunchDelay--;

  Sprout::update();
}

//----------------------------------------------------------------------------------------------------------------------

void Sprout::draw (cairo::Context ctx)
{
	if (!isGrowing())
	  return;

	float ageLerp = 1.0 - mLifespan / (float)mTotalLifespan;
	float radius = lerp( mStartEllipseRadius, mEndEllipseRadius, ageLerp );
	ColorA drawColor = lerp( mStartColor, mEndColor, ageLerp );
	ctx.setSourceRgba( drawColor.r, drawColor.g, drawColor.b, 0.075f );
	ctx.circle( mPos, ( 5.0f * mScale + radius * 1.3f ) / 2 );
	ctx.fill();
	ctx.setSource( drawColor );
	ctx.circle( mPos, radius / 2 );
	ctx.fill();
}

//----------------------------------------------------------------------------------------------------------------------

void Branch::draw (cairo::Context ctx)
{
  Sprout::draw (ctx);

  // Draw all branches and blossoms
	for( vector<Sprout*>::iterator it = mSproutes.begin(); it != mSproutes.end(); it++ ) {
		(*it)->draw (ctx);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void Branch::createBranch (const Vec2f &origin, float baseHue, ColorA flowerColor)
{
	float changeProb = 0.05f + Rand::randFloat( 0.05f );
	float flowerProb = Rand::randFloat( FLOWER_PROB/4.0f, FLOWER_PROB );
	float startRadius = 5 + Rand::randFloat( 5 );
	float endRadius = Rand::randFloat( 1 );
	int lifespan = 100 + Rand::randInt( 100 );
	float speed = Rand::randFloat( 0.9f, 1.0f );
	float hue = baseHue + Rand::randFloat( 0.1f );
	float sat = 0.5 + Rand::randFloat( 0.5f );
	float brightness = 1.0f;
	float opacity = 0.5 + Rand::randFloat( 0.5 );
	ColorA startColor = ColorA( CM_HSV, hue, sat, brightness, opacity );
	ColorA endColor = ColorA( CM_HSV, hue + 0.1, sat, brightness / 2.0, opacity );
	float scale = Rand::randFloat( 0.5f, 1.0f );

	float angleDelta;
	if (Rand::randBool())
	  angleDelta = Rand::randFloat (0.02f, 0.15f);
  else
	  angleDelta = Rand::randFloat (-0.02f, -0.15f);


	mSproutes.push_back (new Branch (origin, lifespan, speed, Rand::randFloat( 6.28f ), angleDelta, changeProb, 
                                   flowerProb, startRadius, endRadius, startColor, endColor, flowerColor, scale));
}


//----------------------------------------------------------------------------------------------------------------------

float Sprout::randomHue()
{
	if( Rand::randFloat( 0.6666666666f ) > 0.5f ) // in [ 0, 60 ]
		return Rand::randFloat( 0 / 360.0f, 60.0f / 360.0f );
	else
		return Rand::randFloat( 190.0f / 360.0f, 360.0f / 360.0f );
}

//----------------------------------------------------------------------------------------------------------------------

