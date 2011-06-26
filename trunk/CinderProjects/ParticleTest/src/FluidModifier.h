#include "cinder/app/AppBasic.h"
#include "Modifier.h"
#include "Emitter.h"

#include "cinder/Vector.h"

using namespace ci;

#define IX(i,j) ((i) + (j)*(mResolution+2))
#define SWAP(x0,x) {float *tmp=x0;x0=x;x=tmp;}

class FluidModifier : public Modifier 
{
public:
	FluidModifier (app::AppBasic *app, Emitter *emitter, size_t resolution, const Vec3f& position, const float width, const float height);
	
	virtual ~FluidModifier ();

  inline void update();

  inline void apply(Particle *const particle);

private:
  Vec2f mouseToWorld (const Vec2i& mousePosition);

  void applyMovement (const Vec2f& position, const Vec2f& speed);

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

	float *mDensity;
	float *mPrevDensity;

  const Vec3f mPosition;
  const float mHalfWidth;
  const float mHalfHeight;
  const float mVolumeWidth;
  const float mVolumeHeight;

  Vec2f mMousePosition;
  Vec2f mPrevMousePosition;

  Emitter *mEmitter;

  app::AppBasic *mApp;
};