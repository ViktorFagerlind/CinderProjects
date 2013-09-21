#include "SpacewarsApp.h"

#include "cinder/gl/gl.h"

#include "World.h"
#include "MainVessel.h"

#include "MovingCamera.h"
#include "ParticleSystemManager.h"
#include "World.h"
#include "Conversions.h"

#if defined (CINDER_COCOA_TOUCH)
  #include "cinder/MotionManager.h"
#endif


void SpacewarsApp::prepareSettings (Settings *settings)
{
#if !defined (CINDER_COCOA_TOUCH)
  settings->setWindowSize (1536/2, 2048/2);
#endif
}

void SpacewarsApp::setup()
{
  World::getSingleton ();

	m_touchPressed = false;

  m_camera.reset (new MovingCamera (1000.f, 20.f));

  m_mainVessel.reset (new MainVessel ());

#if defined (CINDER_COCOA_TOUCH)
	console() << "gyro available: " << MotionManager::isGyroAvailable() << std::endl;
	MotionManager::enable (60.0f);
#endif

  setupLights ();

  m_touchPosition = Vec2f ((float)getWindowWidth ()/2.f, (float)getWindowHeight () / 2.f);
}

void SpacewarsApp::setupLights ()
{
  shared_ptr<gl::Light> light;

  light.reset (new gl::Light( gl::Light::POINT, 0));

  light->setAmbient  (Color::white ());
	light->setDiffuse  (Color::white ());
	light->setSpecular (Color::white ());
  light->setPosition (Vec3f (-1000, 1000, 0));
  gl::enable (GL_LIGHT0);

  /*
  light.reset (new gl::Light( gl::Light::POINT, 1));

  light->setAmbient  (Color (1, 1, .5f));
	light->setDiffuse  (Color (1, 1, .5f));
	light->setSpecular (Color (1, 1, .5f));
  light->setPosition (Vec3f (1000, 0, 0));
  gl::enable (GL_LIGHT1);*/

}


void SpacewarsApp::mouseDown( MouseEvent event )
{
	m_touchPressed = true;
}

void SpacewarsApp::mouseUp( MouseEvent event )
{
	m_touchPressed = false;
}

void SpacewarsApp::mouseMove( MouseEvent event )
{
  /*
  console() << "mx:     " << event.getPos().x << std::endl;
  console() << "my:     " << event.getPos().y << std::endl;
  console() << "width:  " << getWindowWidth () << std::endl;
  console() << "height: " << getWindowHeight () << std::endl;
  */

	m_touchVelocity = event.getPos() - m_touchPosition;
	m_touchPosition = event.getPos();
}

void SpacewarsApp::mouseDrag( MouseEvent event )
{
	mouseMove (event);
}

void SpacewarsApp::touchesBegan( TouchEvent event )
{
  //console() << "Began: " << event << std::endl;

  m_touchPressed = true;

  vector<TouchEvent::Touch>::const_iterator firstTouch = event.getTouches().begin();

	m_touchPosition = firstTouch->getPos ();
  m_touchVelocity = Vec2f (0,0);
}


void SpacewarsApp::touchesMoved( TouchEvent event )
{
  //console() << "Moved: " << event << std::endl;

  vector<TouchEvent::Touch>::const_iterator firstTouch = event.getTouches().begin();

  m_touchVelocity = firstTouch->getPos () - m_touchPosition;
	m_touchPosition = firstTouch->getPos ();
}

void SpacewarsApp::touchesEnded( TouchEvent event )
{
  //console() << "Ended: " << event << std::endl;

	m_touchPressed = getActiveTouches ().size () > 0;
}


void SpacewarsApp::keyDown(KeyEvent event) 
{
  m_camera->keyDown (event);
}


Vec2f SpacewarsApp::mouseToWorld (const Vec2f& mousePos)
{
  float left, top, right, bottom, nearz, farz;

  const CameraPersp& cam = m_camera->getCam ();

  float distanceWidthScaler = 2.f * sinf (toRadians (cam.getFov ()/2.f));

  // ??? Varför behövs detta (speciellt *2) ???
  cam.getFrustum (&left, &top, &right, &bottom, &nearz, &farz);
  distanceWidthScaler *= 2.f * -left;

  const float worldWidth  = cam.getEyePoint().z*distanceWidthScaler;
  const float worldHeight = worldWidth / getWindowAspectRatio ();

  return Vec2f ((float)mousePos.x  / (float)getWindowWidth ()  * worldWidth - worldWidth/2.f,
                (float)-mousePos.y / (float)getWindowHeight () * worldHeight + worldHeight/2.f);
/*
  float u = ((float) mousePos.x) / getWindowWidth ();
  float v = ((float) (getWindowHeight () - mousePos.y)) / getWindowHeight ();
  Ray ray = m_camera->getCam ().generateRay (u, v, getWindowAspectRatio ());
    return Conversions::Vec3fTo2f (ray.calcPosition (1200.f));
*/
}


void SpacewarsApp::update()
{
  static uint32_t frameCount = 0;

  float dt = 1.0f / getFrameRate ();

  if ((frameCount % 60) == 0)
	  console() << "FPS: " << getAverageFps () << std::endl;

  ParticleSystemManager::getSingleton().update ();

  Vec2f touchPos = mouseToWorld (m_touchPosition);

  m_mainVessel->update (dt, touchPos);

  World::getSingleton ().update (dt);

  frameCount++;
}

void SpacewarsApp::draw()
{
	gl::clear (Color (0, 0, 0));	

  // Setup camera
  m_camera->setMatrices ();

	gl::enableDepthRead (); 

//  Vec3f p = Conversions::Vec2fTo3f (mouseToWorld (m_touchPosition));
//  gl::drawSphere (p, 10);

  // Draw solid objects
	gl::enable (GL_LIGHTING);

  m_mainVessel->draw ();

  World::getSingleton ().draw ();

	gl::disable (GL_LIGHTING);

  // Draw particle system
	gl::enableAlphaBlending ();
	gl::disableDepthWrite ();
	gl::enableAdditiveBlending ();

  ParticleSystemManager::getSingleton().draw ();

	gl::enableDepthWrite (); 
	gl::disableAlphaBlending ();
}

#if defined (CINDER_COCOA_TOUCH)
CINDER_APP_NATIVE (SpacewarsApp, RendererGl(0))
#else
CINDER_APP_NATIVE (SpacewarsApp, RendererGl)
#endif
