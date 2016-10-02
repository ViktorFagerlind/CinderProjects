#include "VfBaseApp.h"
#include "MyStrings.h"


using namespace ci;
using namespace ci::app;
using namespace std;


void VfBaseApp::prepareSettings (Settings *settings)
{
  settings->setWindowSize (1280, 768);
  settings->setFullScreen ();
}

void VfBaseApp::setup (const float cameraDistance, const float cameraStepSize, gl::Fbo::Format fboFormat)
{
  fboFormat.setSamples (8); // 8x antialiasing
  m_frameBuffer.reset (new gl::Fbo (getWindowWidth (), getWindowHeight (), fboFormat));

  mCamera.reset (new MovingCamera (cameraDistance, cameraStepSize));

  m_frameCount        = 0;
  m_wireFrameMode     = false;
  m_paused            = false;
  m_savingVideo       = false;
}


void VfBaseApp::keyDown (KeyEvent event)
{
  char c = event.getChar();

  switch (c)
  {
    case 'w':
      m_wireFrameMode = !m_wireFrameMode;
      break;

    case 'p':
      m_paused = !m_paused;
      break;

    case 's':
    {
      writeImage (MyString::getFrameNumber ("ScreenShot", m_frameCount) + ".png", copyWindowSurface ());

      break;
    }

    case 'v':
    {
      m_savingVideo = true;

	    qtime::MovieWriter::Format format;
      std::string dir = "D:\\Libraries\\Documents\\Programming\\CppGraphics\\CinderDevelopment\\CinderProjects\\Release\\";

	    if (qtime::MovieWriter::getUserCompressionSettings (&format, loadImage ("../Media/Images/flare.png")))
		    m_movieWriter = qtime::MovieWriter (dir + "saveVideo.mov", 1280, 768, format);

      break;
    }
  }

  mCamera->keyDown (event);
}

void VfBaseApp::mouseMove (MouseEvent event)
{
  mCamera->mouseMove (event);
}

void VfBaseApp::drawToScreen ()
{
  gl::disableDepthRead ();
	gl::disableAlphaBlending();


  gl::color (1,1,1);
  gl::setViewport (getWindowBounds ());
  gl::setMatricesWindow (getWindowSize (), false);
  gl::draw (m_frameBuffer->getTexture(), m_frameBuffer->getTexture().getBounds(), getWindowBounds());

  m_frameCount++;
  if ((m_frameCount % 10) == 0)
    console() << "FPS: " << getAverageFps() << std::endl;

  // Save video
  if (m_savingVideo)
  {
    static int currentImage = 0;
    if ((currentImage%2) == 0)
    {
      Surface surface (m_frameBuffer->getTexture());
      ip::flipVertical (&surface);

      if (m_savingVideo)
        m_movieWriter.addFrame (surface);
    }
    currentImage++;
  }

  Misc::CheckForOpenGlError ();
}

