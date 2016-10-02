#pragma once

#include "cinder/Color.h"
#include "cinder/Vector.h"
#include "cinder/gl/gl.h"

using namespace ci;

const float Particle_fullLife_C = 100.0;

class ParticleDrawerInterface;

class Particle
{
  friend class ParticleDrawerInterface;

protected:
  Particle ();

public:
  virtual ~Particle ();

  const   vec3&    getPosition   () 						                   {return mPosition;};

  const   vec3&    getVelocity   ()                         const {return mVelocity;};
          void      setVelocity   (vec3& velocity) 			         {mVelocity = velocity;};

          float     getLife       ()                         const {return mLife;};
          void      decreaseLife  (const float lifeDecrease)       {mLife -= lifeDecrease;};
  
          void      scaleSize     (const float scalar)             {mCurrentSize = scalar * mOriginalSize;}
  
          float     getOpacity    () 				                       {return mColor.a;}
          void      setOpacity    (const float opacity)            {mColor.a = opacity;}
  
  const   ColorAf&  getColor      () 				                       {return mColor;}
          void      setColor      (const ColorAf& color)           {mColor = color;}
  
          // Method to apply a force vector to the Particle object
          void      applyForce    (const vec3& force);

  virtual void      define (const vec3& position, float size, vec3 velocity);
  virtual void      update        ();

// --------------------------------------------------------------------------------------------------------------------

public:
  bool    mIsDead;
  vec3   mPosition;
  ColorAf mColor;
  float   mCurrentSize;

private:
  vec3   mVelocity;
  vec3   mAcceleration;
  
  float   mLife;
  float   mOriginalSize;
};
