#pragma once

#include "cinder/Color.h"
#include "cinder/Vector.h"
#include "cinder/gl/gl.h"

using namespace ci;

const float Particle_fullLife_C = 100.0;

class Particle
{
public:

  Particle();

  void define(const Vec3f& position, float size, float minVelocity, float maxVelocity);

  const Vec3f& getPosition () 						{return mPosition;};

  const Vec3f& getVelocity () 						{return mVelocity;};
  void setVelocity (Vec3f& velocity) 			{mVelocity = velocity;};

  float  getLife ()                              {return mLife;};
  void   decreaseLife (const float lifeDecrease) {mLife -= lifeDecrease;};
  
  void   scaleSize (const float scalar)   {mCurrentSize = scalar * mOriginalSize;}
  
  float  getOpacity () 				            {return mColor.a;}
  void   setOpacity (const float opacity) {mColor.a = opacity;}
  
  const ColorAf&  getColor () 				            {return mColor;}
  void            setColor (const ColorAf& color) {mColor = color;}
  
  // Method to apply a force vector to the Particle object
  void applyForce(const Vec3f& force);

  inline void update()
  {
    mVelocity += mAcceleration;
    
    mPosition += mVelocity;

    mAcceleration = Vec3f::zero(); // Kill the acceleration for each update

    mIsDead = mLife <= 0.0;
  }


  inline void draw()
  {
  /* for profiling...
	  glVertex3f(mPosition.x, mPosition.y, mPosition.z); */
  
	  glColor4f( mColor.r, mColor.g, mColor.b, mColor.a);

    glTexCoord2f(0, 0);
	  glVertex3f(mPosition.x-mCurrentSize, mPosition.y-mCurrentSize, mPosition.z);

    glTexCoord2f(1, 0);
	  glVertex3f(mPosition.x+mCurrentSize, mPosition.y-mCurrentSize, mPosition.z);

    glTexCoord2f(1, 1);
	  glVertex3f(mPosition.x+mCurrentSize, mPosition.y+mCurrentSize, mPosition.z);

    glTexCoord2f(0, 1);
	  glVertex3f(mPosition.x-mCurrentSize, mPosition.y+mCurrentSize, mPosition.z);
  }



// --------------------------------------------------------------------------------------------------------------------

public:
  bool    mIsDead;

private:
  Vec3f   mPosition;
  Vec3f   mVelocity;
  Vec3f   mAcceleration;
  ColorAf mColor;
  
  float   mLife;
  float   mOriginalSize;
  float   mCurrentSize;
};
