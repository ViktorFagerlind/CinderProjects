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

  const   Vec3f&    getPosition   () 						                   {return mPosition;};

  const   Vec3f&    getVelocity   ()                         const {return mVelocity;};
          void      setVelocity   (Vec3f& velocity) 			         {mVelocity = velocity;};

          float     getLife       ()                         const {return mLife;};
          void      decreaseLife  (const float lifeDecrease)       {mLife -= lifeDecrease;};
  
          void      scaleSize     (const float scalar)             {mCurrentSize = scalar * mOriginalSize;}
  
          float     getOpacity    () 				                       {return mColor.a;}
          void      setOpacity    (const float opacity)            {mColor.a = opacity;}
  
  const   ColorAf&  getColor      () 				                       {return mColor;}
          void      setColor      (const ColorAf& color)           {mColor = color;}
  
          // Method to apply a force vector to the Particle object
          void      applyForce    (const Vec3f& force);

  virtual void      define (const Vec3f& position, float size, Vec3f velocity);
  virtual void      update        ();

// --------------------------------------------------------------------------------------------------------------------

public:
  bool    mIsDead;
  Vec3f   mPosition;
  ColorAf mColor;
  float   mCurrentSize;

private:
  Vec3f   mVelocity;
  Vec3f   mAcceleration;
  
  float   mLife;
  float   mOriginalSize;
};
