#pragma once

#include "cinder/Color.h"
#include "cinder/Vector.h"

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

  float  getLife () 						          {return mLife;};
  void   setLife (const float life) 		  {mLife = life;};
  
  void   scaleSize (const float scalar)   {mCurrentSize = scalar * mOriginalSize;}
  
  float  getOpacity () 				            {return mColor.a;}
  void   setOpacity (const float opacity) {mColor.a = opacity;}
  
  const ColorAf&  getColor () 				            {return mColor;}
  void            setColor (const ColorAf& color) {mColor = color;}
  
  // Method to apply a force vector to the Particle object
  void applyForce(const Vec3f& force);

  void update();

  void draw();

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
