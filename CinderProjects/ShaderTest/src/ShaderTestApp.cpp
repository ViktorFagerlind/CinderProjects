#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include "cinder/gl/Vbo.h"
#include "cinder/ObjLoader.h"

#include "ShaderHelper.h"
#include "Amoeba.h"
#include "MovingCamera.h"
#include "Macros.h"
#include "Miscellaneous.h"
#include "Tube.h"

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

  bool mPaused;
  bool mWireFrameMode;

  shared_ptr<Amoeba> mAmoeba;

  uint32_t frameCount;
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

  float p1[] = {-1000, 5000, 0};

  glLightfv(GL_LIGHT0, GL_SPECULAR, white);
  glLightfv(GL_LIGHT0, GL_DIFFUSE,  white);
  glLightfv(GL_LIGHT0, GL_AMBIENT,  white);
	glLightfv(GL_LIGHT0, GL_POSITION, p1);
	glEnable (GL_LIGHT0);

  gl::enableDepthRead ();
  gl::enableDepthWrite ();

  mCamera.reset (new MovingCamera(50.0f, 1.0f));

  mAmoeba.reset (new Amoeba (5.f));

  frameCount     = 0;
  mWireFrameMode = false;
  mPaused        = false;
}


void ShaderTestApp::keyDown (KeyEvent event)
{
  char c = event.getChar();

  switch (c)
  {
  case 'w':
    if (mWireFrameMode)
      gl::disableWireframe ();
    else
      gl::enableWireframe ();

    mWireFrameMode = !mWireFrameMode;
    break;
  case 'p':
    mPaused = !mPaused;
    break;
  case 'x':
    mAmoeba->rotate (Matrix44<float>::createRotation (Vec3f(0,0,1),  5.f * (float)M_PI / 180.f));
    break;
  case 'c':
    mAmoeba->rotate (Matrix44<float>::createRotation (Vec3f(0,0,1), -5.f * (float)M_PI / 180.f));
    break;
  case 'a':
    mAmoeba->rotate (Matrix44<float>::createRotation (Vec3f(1,0,0),  5.f * (float)M_PI / 180.f));
    break;
  case 'z':
    mAmoeba->rotate (Matrix44<float>::createRotation (Vec3f(1,0,0), -5.f * (float)M_PI / 180.f));
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
  if (mPaused)
    return;

  mAmoeba->update ();
}

void ShaderTestApp::draw()
{
	// clear out the window with black
	gl::clear (Color (0, 0, 0)); 

  // Setup camera
  mCamera->setMatrices ();

  mAmoeba->draw ();

  Misc::CheckForOpenGlError ();

  frameCount++;
  if ((frameCount % 10) == 0)
    console() << "FPS: " << getAverageFps() << std::endl;
}

CINDER_APP_BASIC( ShaderTestApp, RendererGl )
