#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/app/KeyEvent.h"
#include "cinder/app/MouseEvent.h"
#include "cinder/gl/gl.h"

#include "cinder/Vector.h"
#include "cinder/Camera.h"

#include "GameWorld.h"

using namespace ci;
using namespace ci::app;

class MovingCamera
{
public:  
  MovingCamera (Vec3f eye, Vec3f center)
  {
    mApp        = AppBasic::get ();
  
    mEye        = eye;
    mCenter     = center;
  
    mAngle      = 0.0f;
    mAngleDest  = 0.0f;
    mDist       = 500.0f;
    mDistDest   = 500.0f;
  }

  void setEye (Vec3f e)
  {
    mEye = e;
  }

  void setCenter (Vec3f c)
  {
    mCenter = c;
  }

  void mouseMove (MouseEvent event)
  {
	  static bool firstMouseMove = true;
	  if (!firstMouseMove)
		  mLastMouse = mCurrentMouse;
	  else 
    {
		  mLastMouse = event.getPos();
		  firstMouseMove = false;
	  }
	  mCurrentMouse = event.getPos();
	
	  mAngleDest -=  (mCurrentMouse.x - mLastMouse.x) * 0.025f;
	
	  mEye.y = mCurrentMouse.y - (getWindowHeight() * 0.5f);
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
    
    switch (event.getNativeKeyCode ())
    {
      case KeyEvent::KEY_UP: move.x -= stepSize; break;
      case KeyEvent::KEY_DOWN: move.x += stepSize; break;
      case 'w': move.y += stepSize; break;
      case 's': move.y -= stepSize; break;
    }
  }
  
  void setViewMatrix ()
  {
    mAngle -= (mAngle - mAngleDest) * 0.1f;
    mDist  -= (mDist - mDistDest) * 0.1f;
  
    mEye.x = sin (mAngle) * mDist;
    mEye.z = cos (mAngle) * mDist;
  
    mCam.lookAt (mEye, mCenter);
    mCam.setPerspective  (60.0f, mApp->getWindowAspectRatio(), 1, 20000);
    gl::setMatrices (mCam);
  }

private:
  CameraPersp mCam;

  AppBasic *mApp;

	Vec2f			mLastMouse;
	Vec2f			mCurrentMouse;

  Vec3f mEye;
  Vec3f mCenter;
  Vec3f mEyeNormal;

  float mAngle;
  float mAngleDest;
  float mDist;
  float mDistDest;
};
