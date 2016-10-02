#include "SpacewarsApp.h"

#include "cinder/gl/gl.h"

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
  World::getSingleton ().setup ();

	m_touchPressed = false;

  gl::disableVerticalSync ();
  setFrameRate (60);

#if defined (CINDER_COCOA_TOUCH)
	console() << "gyro available: " << MotionManager::isGyroAvailable() << std::endl;
	MotionManager::enable (60.0f);
#endif

  setupLights ();

  m_touchPosition = vec2 ((float)getWindowWidth ()/2.f, (float)getWindowHeight () / 2.f);
}

void SpacewarsApp::setupLights ()
{
  shared_ptr<gl::Light> light;

  light.reset (new gl::Light( gl::Light::POINT, 0));

  light->setAmbient  (Color::white ());
	light->setDiffuse  (Color::white ());
	light->setSpecular (Color::white ());
  light->setPosition (vec3 (-1000, 1000, 0));
  gl::enable (GL_LIGHT0);

  /*
  light.reset (new gl::Light( gl::Light::POINT, 1));

  light->setAmbient  (Color (1, 1, .5f));
	light->setDiffuse  (Color (1, 1, .5f));
	light->setSpecular (Color (1, 1, .5f));
  light->setPosition (vec3 (1000, 0, 0));
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
  m_touchVelocity = vec2 (0,0);
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
   World::getSingleton ().keyDown (event);
}


void SpacewarsApp::update()
{
  static uint32_t frameCount = 0;

  float dt = 1.0f / getFrameRate ();

  if ((frameCount % 100) == 0)
	  console() << "FPS: " << getAverageFps () << std::endl;

  vec2 touchPos = World::getSingleton ().pixelToWorld (m_touchPosition);

  World::getSingleton ().update (dt, touchPos);

  frameCount++;
}

void SpacewarsApp::draw()
{
  World::getSingleton ().draw ();
}

#if defined (CINDER_COCOA_TOUCH)
CINDER_APP_NATIVE (SpacewarsApp, RendererGl(0))
#else
CINDER_APP_NATIVE (SpacewarsApp, RendererGl)
#endif
