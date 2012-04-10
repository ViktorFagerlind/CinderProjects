#include "MovingCamera.h"

MovingCamera::MovingCamera (float distance)
{
  mStartingDistance = distance;

  reset ();
}

void MovingCamera::reset ()
{
  mApp        = AppBasic::get ();
  
  mEye        = Vec3f(0,0, mStartingDistance);
  mTargetVec  = Vec3f(0,0,-mStartingDistance);
}

void MovingCamera::mouseMove (MouseEvent event)
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

  float angleY = (mCurrentMouse.x - mLastMouse.x) * 0.015f;
  mTargetVec.rotateY (angleY);
	
	float angleX  = (mLastMouse.y - mCurrentMouse.y) * 0.015f;
  mTargetVec.rotateX (angleX);
}

void MovingCamera::keyDown (KeyEvent event)
{
  float stepSize = 20.0f;

  Vec3f move = Vec3f (0, 0, 0);

  Vec3f direction = mTargetVec.normalized ();

  switch (event.getChar ())
  {
    case '.': move += stepSize * direction; break;
    case ',': move -= stepSize * direction; break;

    case ' ': reset (); break;
  }
    
  switch (event.getCode ())
  {
    case KeyEvent::KEY_UP:    move.y += stepSize; break;
    case KeyEvent::KEY_DOWN:  move.y -= stepSize; break;
    case KeyEvent::KEY_LEFT:  move.x -= stepSize; break;
    case KeyEvent::KEY_RIGHT: move.x += stepSize; break;
  }

  mEye += move;
}
  
void MovingCamera::setViewMatrix ()
{
  /*
  mAngle -= (mAngle - mAngleDest) * 0.1f;
  mDist  -= (mDist - mDistDest) * 0.1f;
  
  mEye.x = sin (mAngle) * mDist;
  mEye.z = cos (mAngle) * mDist;
  */
  
  mCam.lookAt (mEye, mEye + mTargetVec);
  mCam.setPerspective  (60.0f, mApp->getWindowAspectRatio(), 1, 20000);
  gl::setMatrices (mCam);
}
