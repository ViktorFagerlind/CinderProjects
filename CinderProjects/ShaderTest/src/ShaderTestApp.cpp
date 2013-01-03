#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include "cinder/gl/Vbo.h"
#include "cinder/ObjLoader.h"

#include "ShaderHelper.h"
#include "PhongMaterial.h"
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
  shared_ptr<MovingCamera>  mCamera;
  shared_ptr<PhongMaterial> mMaterial;

  shared_ptr<Tube>          mTube;

  bool mPaused;
  bool mWireFrameMode;

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

  gl::GlslProg shader = ShaderHelper::loadShader ("../Media/Shaders/tube_vert.glsl", 
                                                  "../Media/Shaders/tube_frag.glsl",
                                                  "../Media/Shaders/tube_geom.glsl",
                                                  GL_POINTS,
                                                  GL_TRIANGLE_STRIP,
                                                  1024);

  mMaterial.reset (new PhongMaterial (shader,
                                      ColorAf (0.05f, 0.1f,  0.05f, 1.0f),  // matAmbient,
                                      ColorAf (0.8f,  0.5f,  0.2f,  1.0f),  // matDiffuse,
                                      ColorAf (1.0f,  0.7f,  0.6f,  1.0f),  // matSpecular,
                                      10.0f));                               // matShininess                         // matShininess

  mCamera.reset (new MovingCamera(30.0f, 1.0f));


  mTube.reset (new Tube ());

  frameCount     = 0;
  mWireFrameMode = false;
  mPaused        = false;
}


void ShaderTestApp::keyDown (KeyEvent event)
{
  char c = event.getChar();

  if (c == 'w')
  {
    if (mWireFrameMode)
      gl::disableWireframe ();
    else
      gl::enableWireframe ();

    mWireFrameMode = !mWireFrameMode;
  }
  else if (c == 'p')
  {
    mPaused = !mPaused;
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
}

void ShaderTestApp::draw()
{
	// clear out the window with black
	gl::clear (Color (0, 0, 0)); 

  // Setup camera
  mCamera->setMatrices ();

/*
  gl::translate (Vec3f (-200.f*3.f/2.f, 0, 0));
  for (uint32_t x = 0; x < 200; x++)
  {
    gl::translate (Vec3f (3, 0, 0));
*/

  mMaterial->bind ();

  mTube->draw (mMaterial->getShader ());

  mMaterial->unbind ();

//  }

  Misc::CheckForOpenGlError ();

  frameCount++;
  if ((frameCount % 10) == 0)
    console() << "FPS: " << getAverageFps() << std::endl;
}

CINDER_APP_BASIC( ShaderTestApp, RendererGl )
