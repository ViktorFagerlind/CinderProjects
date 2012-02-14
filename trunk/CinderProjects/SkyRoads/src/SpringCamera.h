#pragma once

#include "cinder/gl/gl.h"
#include "cinder/Vector.h"
#include "cinder/Camera.h"

using namespace ci;

class SpringCamera
{
public:  
  SpringCamera (float springLength, float springConstant)
  : mSpringLength (springLength),
    mSpringConstant (springConstant)
  {
    mTarget   = Vec3f (0, 0, 0);
    mPosition = Vec3f (0, 0, 1000);

    mCamera.setFarClip (100000);
  }

  void setTarget (Vec3f target)
  {
    mTarget = target;
  }
  
  void update ()
  {
    Vec3f differance = mTarget - mPosition;
    
    // Do we move the camera?
    float distance = differance.length (); 
    if (distance > mSpringLength)
    {
      differance.normalize ();
      Vec3f addition = (distance - mSpringLength) * mSpringConstant * differance;
      
      mPosition += addition;
    }
    
    
    mCamera.lookAt(mPosition, mTarget, Vec3f::yAxis());

    gl::setMatrices (mCamera);
  }

private:
  float mSpringLength;
  float mSpringConstant;

  Vec3f mTarget;
  Vec3f mPosition;

  CameraPersp mCamera;
};
