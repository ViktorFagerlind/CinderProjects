#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
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

  March *mMarch;

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

//	int32_t maxGeomOutputVertices;
//	glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT, &maxGeomOutputVertices);

	try {
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


  mPaused        = false;
  mWireFrameMode = false;

  mMarch = new March ();

  mBallRadius = 40.0f;

  for (int i=0; i<10; i++)
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

  for (int i=0; i<mBallPositions.size (); i++)
  {
    float centerEndgeDist = 250.0f-mBallRadius*2.0f;

    if (mBallPositions[i].x > centerEndgeDist || mBallPositions[i].x < -centerEndgeDist)
      mBallVelocities[i].x = -mBallVelocities[i].x;
    if (mBallPositions[i].y > centerEndgeDist || mBallPositions[i].y < -centerEndgeDist)
      mBallVelocities[i].y = -mBallVelocities[i].y;
    if (mBallPositions[i].z > centerEndgeDist || mBallPositions[i].z < -centerEndgeDist)
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

  blobShader.bind ();

  //uniform vec3 ballPositions[100];
  //uniform int  nofBalls;
  int nofBalls = mBallPositions.size ();
  blobShader.uniform ("ballPositions", mBallPositions.data (), nofBalls);
  blobShader.uniform ("nofBalls",      nofBalls);
  blobShader.uniform ("radiusSquared", mBallRadius*mBallRadius);
  

  mMarch->draw ();

  blobShader.unbind ();
}

CINDER_APP_BASIC( BlobApp, RendererGl )
