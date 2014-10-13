#include "VfBaseApp.h"

#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"
#include "cinder/Rand.h"
#include "cinder/gl/Vbo.h"
#include "cinder/ObjLoader.h"

#include "ShaderHelper.h"
#include "Star.h"
#include "MovingCamera.h"
#include "Macros.h"
#include "Miscellaneous.h"
#include "BloomEffect.h"

#include "ParticleSystemManager.h"
#include "CommonModifier.h"
#include "ColorModifier.h"
#include "GravityModifier.h"
#include "PointGravityModifier.h"
#include "VortexModifier.h"
#include "PerlinModifier.h"


using namespace ci;
using namespace ci::app;
using namespace std;

class SpaceVisualiserApp : public VfBaseApp 
{
public:
	void setup();
  void keyDown (KeyEvent event);
  void mouseDown (MouseEvent event);

	void update();

	void draw();

private:
  std::vector<shared_ptr<Star>> m_Stars;

  shared_ptr<BloomEffect>       m_bloomEffect;
};

void SpaceVisualiserApp::setup()
{
  VfBaseApp::setup (130.0f, 1.0f);

  float white[] = {.8f, .8f, .8f , 0.0f};

  float p1[] = {-1000, 1000, 0};

  glLightfv(GL_LIGHT0, GL_SPECULAR, white);
  glLightfv(GL_LIGHT0, GL_DIFFUSE,  white);
  glLightfv(GL_LIGHT0, GL_AMBIENT,  white);
	glLightfv(GL_LIGHT0, GL_POSITION, p1);
	glEnable (GL_LIGHT0);


  ParticleSystem *particleSystem = new ParticleSystem ("../Media/Images/Particle1.jpg");

  particleSystem->addModifier (new CommonModifier (0.4f,     // lifeChange
                                                   1.0f,     // relativeStartSize
                                                   1.0));    // relativeEndSize);

  particleSystem->addModifier (new ColorModifier (ColorAf (.6f, .5f, .9f, .7f),   // startColor
                                                  ColorAf (.5f, .6f, .9f, .5f),   // middleColor
                                                  ColorAf (.2f, .8f, .9f, 0.f),   // endColor
                                                  0.7f));

  particleSystem->addModifier (new PerlinModifier (5.0f, 0.02f, 0.03f, 0.003f));

  ParticleSystemManager::getSingleton ().addParticleSystem (particleSystem);


  for (uint32_t i=0; i<6; i++)
  {
    shared_ptr<Star> Star (new Star (particleSystem,                      // System
                                     Rand::randFloat (4.f, 6.f),          // Size
                                     Vec3f(Rand::randFloat(-50,50),     // Position
                                           Rand::randFloat(-50,50),
                                           Rand::randFloat(-50,50))));

    m_Stars.push_back (Star);
  }

  m_bloomEffect.reset (new BloomEffect (getWindowWidth()/4, getWindowHeight()/4, getWindowWidth(), getWindowHeight()));


//  hideCursor ();
}

void SpaceVisualiserApp::mouseDown (MouseEvent event)
{
  for (uint32_t i = 0; i<m_Stars.size (); i++)
    m_Stars[i]->bounce ();
}

void SpaceVisualiserApp::keyDown (KeyEvent event)
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
  }

}


void SpaceVisualiserApp::update()
{
  if (m_paused)
    return;

  for (uint32_t i=0; i<m_Stars.size (); i++)
    m_Stars[i]->update ();

  ParticleSystemManager::getSingleton ().update ();
}

void SpaceVisualiserApp::draw()
{
  // Draw to frame buffer from now on
  m_frameBuffer->bindFramebuffer();

	// clear the window with black
  gl::clear (Color::black ());//  Color (0.03f, 0.05f, 0.07f));

  gl::enableDepthRead ();
  gl::enableDepthWrite ();

  if (m_wireFrameMode)
    gl::enableWireframe ();

  // Setup camera
  mCamera->setMatrices ();

  for (uint32_t i=0; i<m_Stars.size (); i++)
    m_Stars[i]->draw ();

  // Draw particle system
  gl::disableDepthWrite ();
  gl::enableAdditiveBlending ();

  ParticleSystemManager::getSingleton ().draw ();

  gl::disableAlphaBlending ();
  gl::enableDepthWrite ();

  m_frameBuffer->unbindFramebuffer();

  // Get blooming effect
  gl::Fbo& bloomedFbo = m_bloomEffect->render (*m_frameBuffer.get());

  // Draw blooming effect
  m_frameBuffer->bindFramebuffer ();
  gl::setViewport (m_frameBuffer->getBounds ());
  gl::setMatricesWindow (getWindowSize (), false);

  gl::enableAdditiveBlending();
  gl::color (1, 1, 1, 0.5f);
  gl::draw (bloomedFbo.getTexture(), bloomedFbo.getTexture().getBounds(), getWindowBounds());
  gl::disableAlphaBlending();
   
  m_frameBuffer->unbindFramebuffer ();

  gl::disableWireframe ();

  VfBaseApp::drawToScreen ();
}

CINDER_APP_BASIC( SpaceVisualiserApp, RendererGl )
