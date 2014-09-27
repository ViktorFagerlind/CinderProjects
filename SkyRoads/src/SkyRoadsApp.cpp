#include "cinder/app/AppBasic.h"

#include "cinder/gl/gl.h"
#include "cinder/Vector.h"
#include "cinder/Camera.h"

#include "SpringCamera.h"
#include "ShaderHelper.h"
#include "Road.h"

#include "PhongMaterial.h"


#include "ParticleSystemManager.h"



using namespace ci;
using namespace ci::app;
using namespace std;

class SkyRoadsApp : public AppBasic {
public:

	void prepareSettings (Settings *settings);

	void setup();
	void setupLights();

	void mouseDown (MouseEvent event);
  void keyDown (KeyEvent event);

	void update();
	void draw();

private:
  Road *mSkyRoads;
  uint32_t mFrameCount;

  SpringCamera *mSpringCamera;

  static const uint32_t mRoadCount;

  shared_ptr<PhongMaterial> mRoadMaterial;

  bool mWireFrameMode;
  bool mPaused;
};

const uint32_t SkyRoadsApp::mRoadCount = 100;


void SkyRoadsApp::prepareSettings (Settings *settings)
{
  settings->setWindowSize (1280, 768);
}

void SkyRoadsApp::keyDown (KeyEvent event)
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
}


void SkyRoadsApp::setupLights()
{
	glEnable( GL_LIGHTING );

  float white[] = {1.0f, 1.0f, 1.0f , 0.0f};
  float p1[] = {-1000, 5000, 0};

  glLightfv(GL_LIGHT0, GL_SPECULAR, white);
  glLightfv(GL_LIGHT0, GL_DIFFUSE,  white);
  glLightfv(GL_LIGHT0, GL_AMBIENT,  white);
	glLightfv(GL_LIGHT0, GL_POSITION, p1);
	glEnable (GL_LIGHT0);

}

void SkyRoadsApp::setup()
{
  mWireFrameMode  = false;
  mPaused         = false;

  mSpringCamera = new SpringCamera (500, 0.01f);

  mSkyRoads    = new Road[mRoadCount];

  for (int i=0; i<mRoadCount; i++)
    mSkyRoads[i].update (); /* Must not be empty since camera uses road blocks */

  mFrameCount = 0;
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

  setupLights ();

  gl::GlslProg phongShader = ShaderHelper::loadShader ("../Media/Shaders/phong_vert.glsl", "../Media/Shaders/phong_frag.glsl");
  
  mRoadMaterial.reset (new PhongMaterial (phongShader,                         // Shader
                                          ColorAf (0.05f, 0.1f,  0.15f, 1.0f), // matAmbient,
                                          ColorAf (0.4f,  0.2f,  0.4f,  1.0f), // matDiffuse,
                                          ColorAf (0.5f,  0.3f,  0.7f,  1.0f), // matSpecular,
                                          6.0f));                              // matShininess

  gl::enableDepthRead ();
}

void SkyRoadsApp::mouseDown( MouseEvent event )
{
}


void SkyRoadsApp::update()
{
  mFrameCount++;
  if ((mFrameCount % 100) == 0)
    console() << "FPS: " << getAverageFps() << std::endl;

  if (mPaused)
    return;

  mSpringCamera->setTarget (mSkyRoads[0].getCurrentRoadEnd ());
  mSpringCamera->update ();

  for (int i=0; i<mRoadCount; i++)
    mSkyRoads[i].update ();

  ParticleSystemManager::getSingleton().update ();
}

void SkyRoadsApp::draw()
{
	// clear the window with black
	gl::clear( Color( 0, 0, 0 ) ); 

  // Draw roads
  mRoadMaterial->bind();

  for (int i=0; i<mRoadCount; i++)
    mSkyRoads[i].draw ();

  mRoadMaterial->unbind();

  // Draw particle system
	gl::disableDepthWrite ();
	gl::enableAdditiveBlending();

  ParticleSystemManager::getSingleton().draw ();

	gl::disableAlphaBlending();
	gl::enableDepthWrite ();

}

CINDER_APP_BASIC( SkyRoadsApp, RendererGl )
