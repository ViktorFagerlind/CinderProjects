#pragma once

#include "cinder/Vector.h"

using namespace ci;

const float Particle_fullLife_C = 100.0;

class Particle
{
public:
  // Basic constructor
  Particle(Vec3f position, float size, Vec3f velocity);

  // Random velocity constructor
  Particle(Vec3f position, float size, float minVelocity, float maxVelocity);

  float  getLife () 						    {return mLife;};
  void   setLife (float life) 		  {mLife = life;};
  
  void   scaleSize (float scalar)   {mCurrentSize = scalar * mOriginalSize;}
  
  float  getOpacity () 				      {return mOpacity;}
  void   setOpacity (float opacity) {mOpacity = opacity;}
  
  // Method to apply a force vector to the Particle object
  void applyForce(Vec3f force);

  void update();

  void draw();

  // Is the particle still useful?
  bool dead();

private:
  Vec3f mPosition;
  Vec3f mVelocity;
  Vec3f mAcceleration;
  
  float   mLife;
  float   mOpacity;
  float   mOriginalSize;
  float   mCurrentSize;
};
