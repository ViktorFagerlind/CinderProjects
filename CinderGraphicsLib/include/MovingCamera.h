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
  MovingCamera (float distance=1000.f, float stepSize=20.f);

  void reset ();

  const CameraPersp& getCam () {return mCam;}

  void setEyePos (const vec3& pos) {mEye = pos;}

  void mouseMove (MouseEvent event);

  void keyDown (KeyEvent event);
  
  float getDistance () const {return mTargetVec.length ();};
  
  void setMatrices ();
  void setModelMatrix ();
  void setProjectionMatrix ();

private:
  CameraPersp mCam;

	float			mStartingDistance;

	vec2			mLastMouse;
	vec2			mCurrentMouse;

  vec3 mEye;
  vec3 mTargetVec;
  vec3 mUpVector;

  float mStepSize;
};
