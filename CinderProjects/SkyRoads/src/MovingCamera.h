#pragma once

#include "cinder/app/KeyEvent.h"
#include "cinder/gl/gl.h"

#include "cinder/Vector.h"
#include "cinder/Camera.h"

#include "GameWorld.h"

using namespace ci;
using namespace ci::app;

class MovingCamera
{
public:  
  MovingCamera (unsigned int screenWidth, unsigned int screenHeight)
  {
    mCamera = new CameraPersp (screenWidth, screenHeight, 25.0 /** M_PI/180.0*/);

    setPositionAndTarget (Vec3f (0, 0, 1100), Vec3f (0,0,0));
    mCamera->setFarClip (100000);
  }

  void setPositionAndTarget (const Vec3f& position, const Vec3f& target)
  {
    mCamera->lookAt(position, target, Vec3f::yAxis());
  }

  void keyDown (KeyEvent event)
  {
    float stepSize = 5.0f;

    Vec3f move = Vec3f (0, 0, 0);

    switch (event.getChar ())
    {
      case 'a': move.x -= stepSize; break;
      case 'd': move.x += stepSize; break;
      case 'w': move.y += stepSize; break;
      case 's': move.y -= stepSize; break;
    }

    mCamera->setEyePoint (mCamera->getEyePoint () + move);
  }
  
  void setViewMatrix ()
  {
    gl::setMatrices (*mCamera);
  }

private:
  CameraPersp *mCamera;
};
