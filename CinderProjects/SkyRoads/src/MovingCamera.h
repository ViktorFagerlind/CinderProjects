#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/app/KeyEvent.h"
#include "cinder/app/MouseEvent.h"
#include "cinder/gl/gl.h"

#include "cinder/Vector.h"
#include "cinder/Camera.h"

using namespace ci;
using namespace ci::app;

class MovingCamera
{
public:  
  MovingCamera (float distance);

  void reset ();

  void mouseMove (MouseEvent event);

  void keyDown (KeyEvent event);
  
  void setMatrices ();
  void setModelMatrix ();
  void setProjectionMatrix ();

private:
  CameraPersp mCam;

  AppBasic *mApp;

	float			mStartingDistance;

	Vec2f			mLastMouse;
	Vec2f			mCurrentMouse;

  Vec3f mEye;
  Vec3f mTargetVec;
  Vec3f mUpVector;

  float mDist;
};
