#pragma once

#include "Modifier.h"
#include "Emitter.h"

#include "cinder/Vector.h"

using namespace ci;

#define IX(i,j) ((i) + (j)*(mResolution+2))
#define SWAP(x0,x) {float *tmp=x0;x0=x;x=tmp;}

class FluidModifier : public Modifier 
{
public:
	FluidModifier (size_t resolution, const vec3& position, const float viscosity, const bool bounded, const float width, const float height);
	
	virtual ~FluidModifier ();

  virtual void updateModifier ();

  inline void apply(Particle *const particle);

  void applyMovement (const vec2& position, const vec2& speed);

private:
  void vel_step (float *u, float *v, float *u0, float *v0, float visc, float dt);

  void dens_step (float *x, float *x0,  float *u, float *v, float diff, float dt);

  void project (float *u, float *v, float *p, float *div );

  void diffuse (int b, float * x, float * x0, float diff, float dt );
  
  void advect (int b, float *d, float *d0, float *u, float *v, float dt);

  void set_bnd (int b, float *x);

  void add_source (float *x, float *s, float dt);

private:
  const int    mResolution;

	const float  mDiffusionConstant;
	const float  mViscosity;

	float *mVelocityX;
	float *mVelocityY;

	float *mPrevVelocityX;
	float *mPrevVelocityY;

	float *mInputVelocityX;
	float *mInputVelocityY;

	float *mDensity;
	float *mPrevDensity;

  const vec3 mPosition;
  const float mHalfWidth;
  const float mHalfHeight;
  const float mVolumeWidth;
  const float mVolumeHeight;

  const bool  mBounded;
};