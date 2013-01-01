#include "MovingCamera.h"

MovingCamera::MovingCamera (float distance, float stepSize)
{
  mStartingDistance = distance;
  mStepSize         = stepSize;

  reset ();
}

void MovingCamera::reset ()
{
  mApp        = AppBasic::get ();
  
  mEye        = Vec3f(0,0, mStartingDistance);
  mTargetVec  = Vec3f(0,0,-mStartingDistance);
  mUpVector   = Vec3f(0,1,0);
}

void MovingCamera::mouseMove (MouseEvent event)
{
  Vec3f forward = mTargetVec.normalized ();
  Vec3f right   = forward.cross (mUpVector);

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
  Vec3f forward = mTargetVec.normalized ();
  Vec3f right   = forward.cross (mUpVector);

  Vec3f move = Vec3f (0, 0, 0);

  switch (event.getChar ())
  {
    case '.': move += mStepSize * forward; break;
    case ',': move -= mStepSize * forward; break;

    case ' ': reset (); break;
  }
    
  switch (event.getCode ())
  {
    case KeyEvent::KEY_UP:    move += mStepSize * mUpVector; break;
    case KeyEvent::KEY_DOWN:  move -= mStepSize * mUpVector; break;
    case KeyEvent::KEY_LEFT:  move -= mStepSize * right;     break;
    case KeyEvent::KEY_RIGHT: move += mStepSize * right;     break;
  }

  mEye += move;
}
  
void MovingCamera::setMatrices ()
{
  mCam.lookAt (mEye, mEye + mTargetVec, mUpVector);
  mCam.setPerspective  (60.0f, mApp->getWindowAspectRatio(), 1, 20000);
  gl::setMatrices (mCam);
}

void MovingCamera::setModelMatrix ()
{
  mCam.lookAt (mEye, mEye + mTargetVec, mUpVector);
  mCam.setPerspective  (60.0f, mApp->getWindowAspectRatio(), 1, 20000);
  gl::setModelView (mCam);
}

void MovingCamera::setProjectionMatrix ()
{
  mCam.lookAt (mEye, mEye + mTargetVec, mUpVector);
  mCam.setPerspective  (60.0f, mApp->getWindowAspectRatio(), 1, 20000);
  gl::setProjection (mCam);
}

