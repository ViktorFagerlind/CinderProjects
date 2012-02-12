#include "cinder/app/AppBasic.h"

#include "cinder/gl/gl.h"
#include "cinder/Vector.h"
#include "cinder/Camera.h"

#include "Road.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class SkyRoadsApp : public AppBasic {
public:
	void prepareSettings (Settings *settings);
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();

	void updateCamera();

private:
  Road *mSkyRoads;
  unsigned int mFrameCount;

  CameraPersp mCam;

  static const unsigned int mRoadCount;
};

const unsigned int SkyRoadsApp::mRoadCount = 10;


void SkyRoadsApp::prepareSettings (Settings *settings)
{
  settings->setWindowSize (1280, 768);
}

void SkyRoadsApp::setup()
{
  mSkyRoads    = new Road[mRoadCount];

  for (int i=0; i<mRoadCount; i++)
    mSkyRoads[i].update (); /* Must not be empty since camera uses road blocks */

  mFrameCount = 0;
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	glEnable( GL_LIGHTING );

  float white[] = {1.0f, 1.0f, 1.0f , 0.0f};
  float red[]   = {1.0f, 0.0f, 0.0f , 0.0f};
  float green[] = {0.0f, 1.0f, 0.0f , 0.0f};
  float blue[]  = {0.0f, 0.0f, 1.0f , 0.0f};
  float black[] = {0.0f, 0.0f, 0.0f , 0.0f};

  glLightfv(GL_LIGHT0, GL_SPECULAR, white);
  glLightfv(GL_LIGHT0, GL_DIFFUSE,  red);
  glLightfv(GL_LIGHT0, GL_AMBIENT,  black);
	glLightfv( GL_LIGHT0, GL_POSITION, Vec3f (-1000,500,0));
	glEnable( GL_LIGHT0 );

  glLightfv(GL_LIGHT1, GL_SPECULAR, white);
  glLightfv(GL_LIGHT1, GL_DIFFUSE,  blue);
  glLightfv(GL_LIGHT1, GL_AMBIENT,  black);
	glLightfv( GL_LIGHT1, GL_POSITION, Vec3f (0, 0,0));
	glEnable( GL_LIGHT1 );

}

void SkyRoadsApp::mouseDown( MouseEvent event )
{
}

void SkyRoadsApp::updateCamera()
{
  Vec3f cameraPosition = Vec3f (0.0f, 0.0f, 2000.0f);

  mCam.setFarClip (100000);

  mCam.lookAt(mSkyRoads[0].getFirstRoadBlock ().mLeft,  // camera
              mSkyRoads[0].getCurrentRoadEnd (),   // center
              mSkyRoads[0].getFirstRoadBlock ().mLeft /*Vec3f::yAxis()*/); // up

  gl::setMatrices (mCam);

}

void SkyRoadsApp::update()
{
  for (int i=0; i<mRoadCount; i++)
    mSkyRoads[i].update ();

  mFrameCount++;
  if ((mFrameCount % 10) == 0)
    console() << "FPS: " << getAverageFps() << std::endl;
}

void SkyRoadsApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 

  updateCamera();

  float mcolor[] = { 1.0f, 0.0f, 0.0f, 1.0f };
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mcolor);

  glMateriali(GL_FRONT, GL_SHININESS, 15);

  for (int i=0; i<mRoadCount; i++)
    mSkyRoads[i].draw ();
}

CINDER_APP_BASIC( SkyRoadsApp, RendererGl )
