#include "FluidModifier.h"

#include "cinder/Rand.h"

FluidModifier::FluidModifier (size_t resolution, const Vec3f& position, const float viscosity, const bool bounded, const float width, const float height)
: mResolution (resolution),
  mDiffusionConstant(100.0f),
	mViscosity (viscosity),
  mBounded (bounded),
  mPosition (position),
  mHalfWidth (width/2.0f),
  mHalfHeight (height/2.0f),
  mVolumeWidth (width/mResolution),
  mVolumeHeight (height/mResolution)
{
  mVelocityX      = new float[(mResolution+2)*(mResolution+2)];
  mVelocityY      = new float[(mResolution+2)*(mResolution+2)];
  mPrevVelocityX  = new float[(mResolution+2)*(mResolution+2)];
  mPrevVelocityY  = new float[(mResolution+2)*(mResolution+2)];
  mInputVelocityX = new float[(mResolution+2)*(mResolution+2)];
  mInputVelocityY = new float[(mResolution+2)*(mResolution+2)];
	mDensity        = new float[(mResolution+2)*(mResolution+2)];
	mPrevDensity    = new float[(mResolution+2)*(mResolution+2)];

  for (int i=0; i<(mResolution+2)*(mResolution+2); i++)
  {
    mVelocityX[i]      = 0.0f;
    mVelocityY[i]      = 0.0f;
    mPrevVelocityX[i]  = 0.0f;
    mPrevVelocityY[i]  = 0.0f;
    mInputVelocityX[i] = 0.0f;
    mInputVelocityY[i] = 0.0f;
	  mDensity[i]        = 0.0f;
	  mPrevDensity[i]    = 0.0f;
  }
}
	
FluidModifier::~FluidModifier ()
{
  delete [] mVelocityX;
  delete [] mVelocityY;
  delete [] mPrevVelocityX;
  delete [] mPrevVelocityY;
	delete [] mDensity;
	delete [] mPrevDensity;
}

void FluidModifier::applyMovement (const Vec2f& position, const Vec2f& speed)
{
  int xCenter = (int)((position.x - mPosition.x + mHalfWidth) / mVolumeWidth);
  int yCenter = (int)((position.y - mPosition.y + mHalfHeight) / mVolumeHeight);

  if (xCenter < 0 || xCenter >= mResolution)
    return;
  if (yCenter < 0 || yCenter >= mResolution)
    return;

  for (int i=-5; i<5; i++)
  {
    for (int j=-5; j<5; j++)
    {
//      mInputVelocityX[IX(xCenter+i, yCenter+j)] = speed.x * 0.1f;
//      mInputVelocityY[IX(xCenter+i, yCenter+j)] = -speed.y * 0.1f;

      mPrevVelocityX[IX(xCenter+i, yCenter+j)] = speed.x * 0.1f;
      mPrevVelocityY[IX(xCenter+i, yCenter+j)] = -speed.y * 0.1f;

//      mPrevDensity[IX(mResolution/2+i, mResolution/2+j)] = 10;
    }
  }
}

inline void FluidModifier::update()
{
  float dt = 1.0f / 60.0f;
/*
  // Add input
  add_source (mPrevVelocityX, mInputVelocityX, 1);
  add_source (mPrevVelocityY, mInputVelocityY, 1);

  // clear input
  memset (mInputVelocityX, 0, sizeof(float) * (mResolution+2)*(mResolution+2));
  memset (mInputVelocityY, 0, sizeof(float) * (mResolution+2)*(mResolution+2));
*/
  // Perform calculations
  vel_step (mVelocityX, mVelocityY, mPrevVelocityX, mPrevVelocityY, mViscosity, dt);
  dens_step (mDensity, mPrevDensity, mVelocityX, mVelocityY, mDiffusionConstant, dt);
}

inline void FluidModifier::apply(Particle *const particle) 
{
  int x = (int)((particle->mPosition.x + mHalfWidth) / mVolumeWidth);
  int y = (int)((particle->mPosition.y + mHalfHeight) / mVolumeHeight);

  if (x < 0 || x >= mResolution)
    return;
  if (y < 0 || y >= mResolution)
    return;

  particle->setVelocity( Vec3f (mVelocityX[IX(x+1, y+1)]*100.0f, mVelocityY[IX(x+1, y+1)]*100.0f, 0.0f));

/*
  particle->setColor (ColorAf(mDensity[IX(x+1, y+1)]/100.0f, 
                              mDensity[IX(x+1, y+1)]/50.0f, 
                              mDensity[IX(x+1, y+1)]/10.0f, mDensity[IX(x+1, y+1)]/1000.0f));
*/
}

void FluidModifier::vel_step (float *u, float *v, float *u0, float *v0, float visc, float dt)
{
  add_source (u, u0, dt); 
  add_source (v, v0, dt);

  SWAP (u0, u); 
  diffuse (1, u, u0, visc, dt);

  SWAP (v0, v); 
  diffuse (2, v, v0, visc, dt);

  project (u, v, u0, v0);

  SWAP (u0, u); 
  SWAP (v0, v);
  advect (1, u, u0, u0, v0, dt); 
  advect (2, v, v0, u0, v0, dt);

  project (u, v, u0, v0);
}

void FluidModifier::dens_step (float *x, float *x0,  float *u, float *v, float diff, float dt)
{
  add_source (x, x0, dt );

  SWAP (x0, x); 
  diffuse (0, x, x0, diff, dt);

  SWAP (x0, x); 
  advect (0, x, x0, u, v, dt);
}


void FluidModifier::project (float *u, float *v, float *p, float *div )
{
  int i, j, k;
  float h = 1.0f/mResolution;

  for ( i=1 ; i<=mResolution ; i++ ) 
  {
    for ( j=1 ; j<=mResolution ; j++ ) 
    {
      div[IX(i,j)] = -0.5f*h*(u[IX(i+1,j)] - u[IX(i-1,j)] + v[IX(i,j+1)] - v[IX(i,j-1)]);
      p[IX(i,j)] = 0;
    }
  }

  set_bnd (0, div); 
  set_bnd (0, p);

  for ( k=0 ; k<20 ; k++ ) 
  {
    for ( i=1 ; i<=mResolution ; i++ ) 
    {
      for ( j=1 ; j<=mResolution ; j++ ) 
      {
        p[IX(i,j)] = (div[IX(i,j)] + p[IX(i-1,j)] + p[IX(i+1,j)] + p[IX(i,j-1)]+p[IX(i,j+1)])/4;
      }
    }
    set_bnd (0, p );
  }

  for ( i=1 ; i<=mResolution ; i++ ) 
  {
    for ( j=1 ; j<=mResolution ; j++ ) 
    {
      u[IX(i,j)] -= 0.5f*(p[IX(i+1,j)]-p[IX(i-1,j)])/h;
      v[IX(i,j)] -= 0.5f*(p[IX(i,j+1)]-p[IX(i,j-1)])/h;
    }
  }
  set_bnd (1, u); 
  set_bnd (2, v );
}

void FluidModifier::diffuse (int b, float * x, float * x0, float diff, float dt )
{
  int i, j, k;

  float a=dt*diff*mResolution*mResolution;

  for ( k=0 ; k<20 ; k++ ) 
  {
    for ( i=1 ; i<=mResolution ; i++ ) 
    {
      for ( j=1 ; j<=mResolution ; j++ ) 
      {
        x[IX(i,j)] = (x0[IX(i,j)] + a * (x[IX(i-1,j)] + x[IX(i+1,j)] + x[IX(i,j-1)] + x[IX(i,j+1)])) / (1+4*a);
      }
    }
  }

  set_bnd (b, x );
}

  
void FluidModifier::advect (int b, float *d, float *d0, float *u, float *v, float dt)
{
  int i, j, i0, j0, i1, j1;
  float x, y, s0, t0, s1, t1, dt0;
  dt0 = dt*mResolution;

  for ( i=1 ; i<=mResolution; i++ ) 
  {
    for ( j=1 ; j<=mResolution; j++ ) 
    {
      x = i - dt0 * u[IX(i,j)];
      y = j - dt0 * v[IX(i,j)];

      if (x < 0.5f) 
        x=0.5f; 

      if (x > mResolution+0.5f) 
        x = mResolution + 0.5f; 

      i0=(int)x; 
      i1=i0+1;

      if (y<0.5f) 
        y=0.5f;
 
      if (y > mResolution+0.5f) 
        y= mResolution + 0.5f; 

      j0=(int)y; 
      j1=j0+1;

      s1 = x-i0; 
      s0 = 1-s1; 
      t1 = y-j0; 
      t0 = 1-t1;
  
      d[IX(i,j)] = s0 * (t0*d0[IX(i0,j0)] + t1*d0[IX(i0,j1)]) + s1*(t0*d0[IX(i1,j0)] + t1*d0[IX(i1,j1)]);
    }
  }
  set_bnd (b, d );
}


void FluidModifier::set_bnd (int b, float *x)
{
  int i;

  // Set no bounds
  if (!mBounded)
    return;

  for ( i=1 ; i<=mResolution; i++ ) 
  {
    x[IX(0  ,i)]           = b==1 ? -x[IX(1,i)]           : x[IX(1,i)];
    x[IX(mResolution+1,i)] = b==1 ? -x[IX(mResolution,i)] : x[IX(mResolution,i)];
    x[IX(i,0  )]           = b==2 ? -x[IX(i,1)]           : x[IX(i,1)];
    x[IX(i,mResolution+1)] = b==2 ? -x[IX(i,mResolution)] : x[IX(i,mResolution)];
  }
  x[IX(0            ,0)]              = 0.5f*(x[IX(1,0  )]+x[IX(0  ,1)]);
  x[IX(0            ,mResolution+1)]  = 0.5f*(x[IX(1,mResolution+1)]+x[IX(0  ,mResolution )]);
  x[IX(mResolution+1,0)]              = 0.5f*(x[IX(mResolution,0  )]+x[IX(mResolution+1,1)]);
  x[IX(mResolution+1,mResolution+1)]  = 0.5f*(x[IX(mResolution,mResolution+1)]+x[IX(mResolution+1,mResolution )]);
}

void FluidModifier::add_source (float *x, float *s, float dt)
{
  int i;
  int size=(mResolution+2)*(mResolution+2);
  
  for ( i=0 ; i<size ; i++ ) 
    x[i] += dt*s[i];
}
