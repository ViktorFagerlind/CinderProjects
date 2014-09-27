#include "VfBaseApp.h"

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
#include "BloomEffect.h"


using namespace ci;
using namespace ci::app;
using namespace std;

class AnemonionApp : public VfBaseApp 
{
public:
	void setup();
  void keyDown (KeyEvent event);

	void update();

	void draw();

private:
  std::vector<shared_ptr<Amoeba>> m_amoebas;

  shared_ptr<BloomEffect>         m_bloomEffect;
};

void AnemonionApp::setup()
{
  VfBaseApp::setup (130.0f, 1.0f);

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

  m_amoebas.push_back (shared_ptr<Amoeba> (new Amoeba (5.f, Vec3f(0,0,0))));
  for (uint32_t i=0; i<20; i++)
  {
    shared_ptr<Amoeba> amoeba(new Amoeba (Rand::randFloat(4.f,6.f),             // Size
                                          Vec3f(Rand::randFloat(-100,100),     // Position
                                                Rand::randFloat(-100,100),
                                                Rand::randFloat(-100,100))));

    m_amoebas.push_back (amoeba);
  }

  m_bloomEffect.reset (new BloomEffect (getWindowWidth()/4, getWindowHeight()/4, getWindowWidth(), getWindowHeight()));


//  hideCursor ();
}


void AnemonionApp::keyDown (KeyEvent event)
{
  VfBaseApp::keyDown (event);

  char c = event.getChar();

  switch (c)
  {
  case 'f':
  {
    setFullScreen (!isFullScreen ());

    gl::Fbo::Format format;
	  format.setSamples (16); // 8x antialiasing
    m_frameBuffer.reset (new gl::Fbo (getWindowWidth(), getWindowHeight(), format));
    m_bloomEffect.reset (new BloomEffect (getWindowWidth()/4, getWindowHeight()/4, getWindowWidth(), getWindowHeight()));
    break;
  }  

#if 0
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
#endif
  }

}


void AnemonionApp::update()
{
  if (m_paused)
    return;

  for (uint32_t i=0; i<m_amoebas.size (); i++)
    m_amoebas[i]->update ();

}

void AnemonionApp::draw()
{
  // Draw to frame buffer from now on
  m_frameBuffer->bindFramebuffer();

	// clear the window with black
	gl::clear (Color (0.03f, 0.05f, 0.07f)); 

  gl::enableDepthRead ();
  gl::enableDepthWrite ();

  if (m_wireFrameMode)
    gl::enableWireframe ();

  // Setup camera
  mCamera->setMatrices ();

  for (uint32_t i=0; i<m_amoebas.size (); i++)
    m_amoebas[i]->draw ();

  m_frameBuffer->unbindFramebuffer();

  // Get blooming effect
  gl::Fbo& bloomedFbo = m_bloomEffect->render (*m_frameBuffer.get());

  // Draw blooming effect
  m_frameBuffer->bindFramebuffer ();
  gl::setViewport (m_frameBuffer->getBounds ());
  gl::setMatricesWindow (getWindowSize (), false);

  gl::enableAdditiveBlending();
  gl::color (1, 1, 1, 0.7f);
  gl::draw (bloomedFbo.getTexture(), bloomedFbo.getTexture().getBounds(), getWindowBounds());
  gl::disableAlphaBlending();
   
  m_frameBuffer->unbindFramebuffer ();

  gl::disableWireframe ();

  VfBaseApp::drawToScreen ();
}

CINDER_APP_BASIC( AnemonionApp, RendererGl )
