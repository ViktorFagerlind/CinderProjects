#include "MovingCamera.h"

using namespace ci::app;

MovingCamera::MovingCamera (float distance, float stepSize)
{
  mStartingDistance = distance;
  mStepSize         = stepSize;

  reset ();
}

void MovingCamera::reset ()
{
  mEye        = vec3(0,0, mStartingDistance);
  mTargetVec  = vec3(0,0,-mStartingDistance);
  mUpVector   = vec3(0,1,0);
}

void MovingCamera::mouseMove (MouseEvent event)
{
  vec3 forward = mTargetVec.normalized ();
  vec3 right   = forward.cross (mUpVector);

	static bool firstMouseMove = true;

	if (!firstMouseMove)
		mLastMouse = mCurrentMouse;
	else 
  {
		mLastMouse = event.getPos();
		firstMouseMove = false;
	}
	mCurrentMouse = event.getPos();

  float angleSide = (mLastMouse.x - mCurrentMouse.x) * 0.005f;
  mTargetVec.rotate (mUpVector, angleSide);
  mUpVector.rotate (mUpVector, angleSide);
	
	float angleUp  = (mLastMouse.y - mCurrentMouse.y) * 0.005f;
  mTargetVec.rotate (right, angleUp);
  mUpVector.rotate (right, angleUp);
}

void MovingCamera::keyDown (KeyEvent event)
{
  vec3 forward = mTargetVec.normalized ();
  vec3 right   = forward.cross (mUpVector);

  vec3 move = vec3 (0, 0, 0);

  switch (event.getChar ())
  {
    case '.': move += mStepSize * forward; break;
    case ',': move -= mStepSize * forward; break;

    case ' ': reset (); break;
  }
    
#if !defined (CINDER_COCOA_TOUCH)
  switch (event.getCode ())
  {
    case KeyEvent::KEY_UP:    move += mStepSize * mUpVector; break;
    case KeyEvent::KEY_DOWN:  move -= mStepSize * mUpVector; break;
    case KeyEvent::KEY_LEFT:  move -= mStepSize * right;     break;
    case KeyEvent::KEY_RIGHT: move += mStepSize * right;     break;
  }
#endif

  mEye += move;
}
  
void MovingCamera::setMatrices ()
{
  mCam.lookAt (mEye, mEye + mTargetVec, mUpVector);
  mCam.setPerspective  (60.0f, getWindowAspectRatio(), 1, 20000);
  gl::setMatrices (mCam);
}

void MovingCamera::setModelMatrix ()
{
  mCam.lookAt (mEye, mEye + mTargetVec, mUpVector);
  mCam.setPerspective  (60.0f, getWindowAspectRatio(), 1, 20000);
  gl::setModelView (mCam);
}

void MovingCamera::setProjectionMatrix ()
{
  mCam.lookAt (mEye, mEye + mTargetVec, mUpVector);
  mCam.setPerspective  (60.0f, getWindowAspectRatio(), 1, 20000);
  gl::setProjection (mCam);
}

