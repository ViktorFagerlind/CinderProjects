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

class PerlinDensity : public DensityInterface
{
  float f (const Vec3f& p)
  {
    Vec2f xz = Vec2f (p.x, p.z);

    return p.y + 5.0f + 40.0f * cos (xz.length () / 20.0f);
  }

};

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

  IsoSurface    *mIsoMesh;
  PhongMaterial *mBlobMaterial;

  PhongMaterial *mSurfaceMaterial;

  uint32_t      frameCount;

  bool          mPaused;
  bool          mWireFrameMode;
  float         mBallRadius;
  vector<Vec3f> mBallPositions;
  vector<Vec3f> mBallVelocities;

  gl::VboMesh   mFunSurface;
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
  gl::GlslProg surfaceShader;
	try {
    //	int32_t maxGeomOutputVertices;
    //	glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT, &maxGeomOutputVertices);
		surfaceShader = gl::GlslProg (loadFile ("../Media/Shaders/phong_vert.glsl"), 
                                  loadFile ("../Media/Shaders/phong_frag.glsl"));
	}	catch (gl::GlslProgCompileExc &exc) {
		std::cout << "Shader compile error: " << std::endl;
		std::cout << exc.what();
	}	catch (...) {
		std::cout << "Unable to load shader" << std::endl;
	}

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
  mBlobMaterial = new PhongMaterial (blobShader,                       // Shader
                                     ColorAf (0.3f, 0.2f, 0.3f, 1.0f), // matAmbient,
                                     ColorAf (0.5f, 0.2f, 0.7f, 1.0f), // matDiffuse,
                                     ColorAf (1.0f, 1.0f, 1.0f, 1.0f), // matSpecular,
                                     50.0f);                           // matShininess

  mSurfaceMaterial = new PhongMaterial (surfaceShader,                    // Shader
                                        ColorAf (0.3f, 0.2f, 0.3f, 1.0f), // matAmbient,
                                        ColorAf (0.5f, 0.2f, 0.7f, 1.0f), // matDiffuse,
                                        ColorAf (1.0f, 1.0f, 1.0f, 1.0f), // matSpecular,
                                        50.0f);                           // matShininess


  mIsoMesh = new IsoSurface (100, 
                             100, 
                             100, 
                             800, 
                             800, 
                             800);

  frameCount = 0;

  mPaused        = false;
  mWireFrameMode = false;

  mBallRadius = 70.0f;

  for (int i=0; i<8; i++)
  {
    mBallPositions.push_back (Vec3f (0,0,0));
    mBallVelocities.push_back (Vec3f (Rand::randFloat(-3.0f, 3.0f),
                                      Rand::randFloat(-3.0f, 3.0f),
                                      Rand::randFloat(-3.0f, 3.0f)));
  }

  // Create function surface
  PerlinDensity pd;
  mIsoMesh->getSurfaceMesh (pd, mFunSurface);
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
    float centerEndgeDistX = mIsoMesh->getWidth ()  / 2.0f - mBallRadius*2.0f;
    float centerEndgeDistY = mIsoMesh->getHeight () / 2.0f - mBallRadius*2.0f;
    float centerEndgeDistZ = mIsoMesh->getDepth ()  / 2.0f - mBallRadius*2.0f;

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

  gl::enableDepthRead ();


  //setOrthoProjection ();
  mCamera->setProjectionMatrix ();

  gl::drawCoordinateFrame (10.0f, 2.0f, 1.0f);

  // Setup light
	float center[] = {-200, 1000, 400, 1};
	glLightfv(GL_LIGHT0, GL_POSITION, center);

//  mMaterial->bind ();

  int nofBalls = mBallPositions.size ();
  mBlobMaterial->getShader ().uniform ("ballPositions", mBallPositions.data (), nofBalls);
  mBlobMaterial->getShader ().uniform ("ballPositions", mBallPositions.data (), nofBalls);
  mBlobMaterial->getShader ().uniform ("nofBalls",      nofBalls);
  mBlobMaterial->getShader ().uniform ("radiusSquared", mBallRadius*mBallRadius);
  
//  mIsoMesh->draw ();

//  mMaterial->unbind ();

  mSurfaceMaterial->bind ();
  gl::draw (mFunSurface);
  mSurfaceMaterial->unbind ();

  frameCount++;
  if ((frameCount % 10) == 0)
  {
    console() << "FPS: " << getAverageFps() << std::endl;
  }

}

CINDER_APP_BASIC( BlobApp, RendererGl )
