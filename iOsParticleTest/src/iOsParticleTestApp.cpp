#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Log.h"

#include "ParticleSystemManager.h"

#include "FireComet.h"
#include "Explosion.h"


using namespace ci;
using namespace ci::app;
using namespace std;

class iOsParticleTestApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
  void mouseDrag( MouseEvent event ) override;
	void update() override;
	void draw() override;
  
private:
  shared_ptr<FireComet> fireComet;
};


void iOsParticleTestApp::setup()
{
  // Camera
//  m_cam.setAspectRatio ((float)getWindowWidth() / (float)getWindowHeight());
//  m_cam.lookAt (vec3 ((float)getWindowWidth()/2.f, (float)getWindowHeight()/2.f, getWindowWidth()),
//                vec3 ((float)getWindowWidth()/2.f, (float)getWindowHeight()/2.f, 0.f));
  
  
  fireComet.reset (new FireComet());
 
}

void iOsParticleTestApp::mouseDown (MouseEvent event)
{
  Explosion::create (vec3 ((float)event.getX(), (float)event.getY(), 0.f));
}

void iOsParticleTestApp::mouseDrag (MouseEvent event)
{
  fireComet->setPosition (vec3 ((float)event.getX(), (float)event.getY(), 0.f));
}

void iOsParticleTestApp::update()
{
  ParticleSystemManager::getSingleton().update();
  
  static uint32_t frameCount = 0UL;
  if (frameCount % 100 == 0 || (getAverageFps() < 30.f && frameCount % 10 == 0) || getAverageFps() < 5.f)
    CI_LOG_V( "fps: " << getAverageFps() << " particle count: " << ParticleSystemManager::getSingleton().getCount());
  frameCount++;
}

void iOsParticleTestApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) );
  
  gl::color(1.f, 1.f, 1.f);
  
  //gl::setMatrices( m_cam );
  gl::setMatricesWindowPersp (getWindowWidth(), getWindowHeight(), 60.f, 10.f, 10000.f, true);
  
  // Draw particle system
  gl::disableDepthWrite ();
  //gl::enableAdditiveBlending ();
  //gl::enableAlphaBlending ();
  
  gl::drawSphere(vec3(0,0,0), 1.f);
  
  ParticleSystemManager::getSingleton ().draw ();
}

//CINDER_APP( iOsParticleTestApp, RendererGl )
CINDER_APP (iOsParticleTestApp, RendererGl, [] (App::Settings *settings)
{
  settings->setWindowSize( 1280, 720 );
  settings->setMultiTouchEnabled (false);
}
)


