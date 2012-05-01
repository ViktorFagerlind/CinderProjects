#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"

#include "../../SkyRoads/src/MovingCamera.h"
#include "Macros.h"
#include "March.h"


using namespace ci;
using namespace ci::app;
using namespace std;

class BlobApp : public AppBasic {
  public:
	void setup();
  void prepareSettings (Settings *settings);

  void setOrthoProjection ();

	void update();
	void draw();

  void keyDown (KeyEvent event);
  void mouseMove (MouseEvent event);

  shared_ptr<MovingCamera> mCamera;

  gl::GlslProg blobShader;
};

void BlobApp::keyDown (KeyEvent event)
{
  mCamera->keyDown (event);
}

void BlobApp::mouseMove (MouseEvent event)
{
  mCamera->mouseMove (event);
}

void BlobApp::setup()
{
  mCamera.reset (new MovingCamera(300.0f));

	try {
		blobShader = gl::GlslProg (loadFile ("../Media/Shaders/blob_vert.glsl"), loadFile ("../Media/Shaders/blob_frag.glsl"));
	}	catch (gl::GlslProgCompileExc &exc) {
		std::cout << "Shader compile error: " << std::endl;
		std::cout << exc.what();
	}	catch (...) {
		std::cout << "Unable to load shader" << std::endl;
	}

  // Initialise light
  float ambient[]  = {0.3f, 0.3f, 0.3f, 1.0f};
  float diffuse[]  = {1.0f, 1.0f, 1.0f, 1.0f};
  float specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
	float center[] = {-200, 1000, 400, 1};
  glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
  glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffuse);
  glLightfv(GL_LIGHT0, GL_AMBIENT,  ambient);
	glLightfv(GL_LIGHT0, GL_POSITION, center);
	glEnable (GL_LIGHT0);

  // Setup material for planets
  float mMatAmbient[4]  = {0.3, 0.1, 0.1, 1};
  float mMatDiffuse[4]  = {1, 0, 0, 1};
  float mMatSpecular[4] = {1, 1, 1, 1};
  float mMatShininess   = 30.0f;
  glMaterialfv (GL_FRONT, GL_AMBIENT,	   mMatAmbient);
  glMaterialfv (GL_FRONT, GL_DIFFUSE,	   mMatDiffuse);
  glMaterialfv (GL_FRONT, GL_SPECULAR,   mMatSpecular);
  glMaterialfv (GL_FRONT, GL_SHININESS, &mMatShininess);
}

void BlobApp::prepareSettings (Settings *settings)
{
  settings->setWindowSize (1600, 1000);
}

void BlobApp::setOrthoProjection ()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-getWindowWidth ()/2, getWindowWidth ()/2, -getWindowHeight ()/2, getWindowHeight ()/2, -1000.0, 1000.0);
}

void BlobApp::update()
{
}

void BlobApp::draw()
{
	// clear the window
	gl::clear (Color (0, 0, 0)); 
  // Setup camera
  mCamera->setModelMatrix ();

  //setOrthoProjection ();
  mCamera->setProjectionMatrix ();

  gl::drawCoordinateFrame (10.0f, 2.0f, 1.0f);

  // Setup light
	float center[] = {-200, 1000, 400, 1};
	glLightfv(GL_LIGHT0, GL_POSITION, center);


  blobShader.bind ();
  March::draw ();
  blobShader.unbind ();
}

CINDER_APP_BASIC( BlobApp, RendererGl )
