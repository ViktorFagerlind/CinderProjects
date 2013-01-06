#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"
#include "cinder/Rand.h"
#include "cinder/gl/Vbo.h"
#include "cinder/ObjLoader.h"

#include "ShaderHelper.h"
#include "Amoeba.h"
#include "MovingCamera.h"
#include "Macros.h"
#include "Miscellaneous.h"
#include "Tube.h"

#include "cinder/qtime/MovieWriter.h"
#include "cinder/ip/Flip.h"


using namespace ci;
using namespace ci::app;
using namespace std;

class ShaderTestApp : public AppBasic 
{
public:
  void prepareSettings (Settings *settings);
	void setup();
  void keyDown (KeyEvent event);
  void mouseMove (MouseEvent event);
	void update();
	void draw();

private:
  shared_ptr<MovingCamera> mCamera;

  bool    m_paused;
  bool    m_savingVideo;
  bool    m_wireFrameMode;

  unique_ptr<gl::Fbo>             m_frameBuffer;

  std::vector<shared_ptr<Amoeba>> m_amoebas;

  qtime::MovieWriter	            m_movieWriter;


  uint32_t m_frameCount;
};

void ShaderTestApp::prepareSettings (Settings *settings)
{
  settings->setWindowSize (1600, 1000);
}

void ShaderTestApp::setup()
{
  float white[] = {1.0f, 1.0f, 1.0f , 0.0f};
  float red[]   = {1.0f, 0.0f, 0.0f , 0.0f};
  float green[] = {0.2f, 0.8f, 0.3f , 0.0f};
  float blue[]  = {0.0f, 0.0f, 1.0f , 0.0f};
  float black[] = {0.0f, 0.0f, 0.0f , 0.0f};

  float p1[] = {-1000, 1000, 0};

  glLightfv(GL_LIGHT0, GL_SPECULAR, white);
  glLightfv(GL_LIGHT0, GL_DIFFUSE,  white);
  glLightfv(GL_LIGHT0, GL_AMBIENT,  white);
	glLightfv(GL_LIGHT0, GL_POSITION, p1);
	glEnable (GL_LIGHT0);

  mCamera.reset (new MovingCamera(130.0f, 1.0f));

  m_amoebas.push_back (shared_ptr<Amoeba> (new Amoeba (5.f, Vec3f(0,0,0))));
  for (uint32_t i=0; i<10; i++)
  {
    shared_ptr<Amoeba> amoeba(new Amoeba (Rand::randFloat(4.f,6.f),             // Size
                                          Vec3f(Rand::randFloat(-100,100),     // Position
                                                Rand::randFloat(-100,100),
                                                Rand::randFloat(-100,100))));

    m_amoebas.push_back (amoeba);
  }

  gl::Fbo::Format format;
	format.setSamples (16); // 8x antialiasing
  m_frameBuffer.reset (new gl::Fbo (getWindowWidth(), getWindowHeight(), format));

  m_frameCount        = 0;
  m_wireFrameMode     = false;
  m_paused            = false;
  m_savingVideo       = false;
}


void ShaderTestApp::keyDown (KeyEvent event)
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

  case 'v':
  {
    m_savingVideo = true;

	  qtime::MovieWriter::Format format;
    std::string dir = "D:\\Libraries\\Documents\\Programming\\CppGraphics\\Cinder\\CinderProjects\\Release\\";

	  if (qtime::MovieWriter::getUserCompressionSettings (&format, loadImage ("../Media/Images/flare.png")))
		  m_movieWriter = qtime::MovieWriter (dir + "saveVideo.mov", 1280, 768, format);

    break;
  }
  case 'x':
    m_amoebas[0]->rotate (Matrix44<float>::createRotation (Vec3f(0,0,1),  5.f * (float)M_PI / 180.f));
    break;
  case 'c':
    m_amoebas[0]->rotate (Matrix44<float>::createRotation (Vec3f(0,0,1), -5.f * (float)M_PI / 180.f));
    break;
  case 'a':
    m_amoebas[0]->rotate (Matrix44<float>::createRotation (Vec3f(1,0,0),  5.f * (float)M_PI / 180.f));
    break;
  case 'z':
    m_amoebas[0]->rotate (Matrix44<float>::createRotation (Vec3f(1,0,0), -5.f * (float)M_PI / 180.f));
    break;

  case 'f':
    m_amoebas[0]->move (Vec3f(-10,  0, 0));
    break;
  case 'h':
    m_amoebas[0]->move (Vec3f( 10,  0, 0));
    break;
  case 't':
    m_amoebas[0]->move (Vec3f(  0, 10, 0));
    break;
  case 'g':
    m_amoebas[0]->move (Vec3f(  0,-10, 0));
    break;

  }

  mCamera->keyDown (event);
}


void ShaderTestApp::mouseMove (MouseEvent event)
{
  mCamera->mouseMove (event);
}

void ShaderTestApp::update()
{
  if (m_paused)
    return;

  for (uint32_t i=0; i<m_amoebas.size (); i++)
    m_amoebas[i]->update ();

}

void ShaderTestApp::draw()
{
  // Draw to frame buffer from now on
  m_frameBuffer->bindFramebuffer();

  gl::enableDepthRead ();
  gl::enableDepthWrite ();

  if (m_wireFrameMode)
    gl::enableWireframe ();

	// clear out the window with black
	gl::clear (Color (0.03, 0.05, 0.07)); 

  gl::pushMatrices();

  // Setup camera
  mCamera->setMatrices ();

  for (uint32_t i=0; i<m_amoebas.size (); i++)
    m_amoebas[i]->draw ();

  Misc::CheckForOpenGlError ();

  gl::popMatrices();

  m_frameBuffer->unbindFramebuffer();

  m_frameCount++;
  if ((m_frameCount % 10) == 0)
    console() << "FPS: " << getAverageFps() << std::endl;

  gl::disableWireframe ();

  gl::disableDepthRead ();

  gl::setMatricesWindow (getWindowSize (), false);
  gl::draw (m_frameBuffer->getTexture(), m_frameBuffer->getTexture().getBounds(), getWindowBounds());

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
}

CINDER_APP_BASIC( ShaderTestApp, RendererGl )
