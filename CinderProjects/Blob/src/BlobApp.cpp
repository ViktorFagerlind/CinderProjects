#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include "cinder/gl/GlslProg.h"

#include "../../SkyRoads/src/MovingCamera.h"
#include "Macros.h"
#include "../../Planets/src/PhongMaterial.h"
#include "IsoSurface.h"


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

  IsoSurface    *mSurface;
  PhongMaterial *mMaterial;

  uint32_t      frameCount;

  bool          mPaused;
  bool          mWireFrameMode;
  float         mBallRadius;
  vector<Vec3f> mBallPositions;
  vector<Vec3f> mBallVelocities;
};

void BlobApp::keyDown (KeyEvent event)
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

void BlobApp::mouseMove (MouseEvent event)
{
  mCamera->mouseMove (event);
}

void BlobApp::setup()
{
  mCamera.reset (new MovingCamera(700.0f));

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

  // Load shaders
  gl::GlslProg blobShader;
	try {
    //	int32_t maxGeomOutputVertices;
    //	glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT, &maxGeomOutputVertices);
		blobShader = gl::GlslProg (loadFile ("../Media/Shaders/blob_vert.glsl"), 
                               loadFile ("../Media/Shaders/blob_frag.glsl"),
                               loadFile ("../Media/Shaders/blob_geom.glsl"),
                               GL_LINES_ADJACENCY_EXT, GL_TRIANGLE_STRIP, 4/*maxGeomOutputVertices*/);
	}	catch (gl::GlslProgCompileExc &exc) {
		std::cout << "Shader compile error: " << std::endl;
		std::cout << exc.what();
	}	catch (...) {
		std::cout << "Unable to load shader" << std::endl;
	}

  // Setup material
  mMaterial = new PhongMaterial (blobShader,                       // Shader
                                 ColorAf (0.3f, 0.2f, 0.3f, 1.0f), // matAmbient,
                                 ColorAf (0.5f, 0.2f, 0.7f, 1.0f), // matDiffuse,
                                 ColorAf (1.0f, 1.0f, 1.0f, 1.0f), // matSpecular,
                                 50.0f);                           // matShininess

  mSurface = new IsoSurface (60, 
                             40, 
                             20, 
                             800, 
                             500, 
                             200);

  frameCount = 0;

  mPaused        = false;
  mWireFrameMode = false;

  mBallRadius = 30.0f;

  for (int i=0; i<15; i++)
  {
    mBallPositions.push_back (Vec3f (0,0,0));
    mBallVelocities.push_back (Vec3f (Rand::randFloat(-3.0f, 3.0f),
                                      Rand::randFloat(-3.0f, 3.0f),
                                      Rand::randFloat(-3.0f, 3.0f)));
  }

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
  if (mPaused)
    return;

  for (uint32_t i=0; i<mBallPositions.size (); i++)
  {
    float centerEndgeDistX = mSurface->getWidth ()  / 2.0f - mBallRadius*2.0f;
    float centerEndgeDistY = mSurface->getHeight () / 2.0f - mBallRadius*2.0f;
    float centerEndgeDistZ = mSurface->getDepth ()  / 2.0f - mBallRadius*2.0f;

    if (mBallPositions[i].x > centerEndgeDistX || mBallPositions[i].x < -centerEndgeDistX)
      mBallVelocities[i].x = -mBallVelocities[i].x;
    if (mBallPositions[i].y > centerEndgeDistY || mBallPositions[i].y < -centerEndgeDistY)
      mBallVelocities[i].y = -mBallVelocities[i].y;
    if (mBallPositions[i].z > centerEndgeDistZ || mBallPositions[i].z < -centerEndgeDistZ)
      mBallVelocities[i].z = -mBallVelocities[i].z;

    mBallPositions[i] += mBallVelocities[i];
  }

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

  mMaterial->bind ();

  int nofBalls = mBallPositions.size ();
  mMaterial->getShader ().uniform ("ballPositions", mBallPositions.data (), nofBalls);
  mMaterial->getShader ().uniform ("ballPositions", mBallPositions.data (), nofBalls);
  mMaterial->getShader ().uniform ("nofBalls",      nofBalls);
  mMaterial->getShader ().uniform ("radiusSquared", mBallRadius*mBallRadius);
  
  mSurface->draw ();

  mMaterial->unbind ();

  frameCount++;
  if ((frameCount % 10) == 0)
  {
    console() << "FPS: " << getAverageFps() << std::endl;
  }

}

CINDER_APP_BASIC( BlobApp, RendererGl )
