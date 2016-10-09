#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Log.h"

#include "ParticleSystemManager.h"

#include "FireComet.h"
#include "Flame.h"
#include "CameraDist.h"
#include "Explosion.h"


using namespace ci;
using namespace ci::app;
using namespace std;

class iOsParticleTestApp : public App {
  public:
	void setup() override;
  void mouseDown( MouseEvent event ) override;
  void mouseUp( MouseEvent event ) override;
  void mouseDrag( MouseEvent event ) override;
	void update() override;
	void draw() override;
  
private:
  enum Effect {FireComet_E, Flame_E};
  
  Effect                 m_currentEffect;
  
  shared_ptr<FireComet>  m_fireComet;
  shared_ptr<Flame>      m_flame;
};


void iOsParticleTestApp::setup()
{
  m_currentEffect = Flame_E;
  
  m_flame.reset (new Flame());
  m_flame->setPosition (vec3 ((float)getWindowWidth() / 2.f, (float)getWindowHeight() / 2.f, 0.f));
}

void iOsParticleTestApp::mouseDown (MouseEvent event)
{
  switch (m_currentEffect)
  {
    case FireComet_E:
      m_fireComet.reset (new FireComet());
      m_fireComet->setPosition (vec3 ((float)event.getX(), (float)event.getY(), 0.f));
      break;
      
    case Flame_E:
      m_flame.reset (new Flame());
      m_flame->setPosition (vec3 ((float)event.getX(), (float)event.getY(), 0.f));
      break;
  }
  
//  Explosion::create (vec3 ((float)event.getX(), (float)event.getY(), 0.f));
}

void iOsParticleTestApp::mouseUp (MouseEvent event)
{
  switch (m_currentEffect)
  {
    case FireComet_E:
      m_fireComet.reset ();
      m_currentEffect = Flame_E;
      break;
      
    case Flame_E:
      m_flame.reset ();
      m_currentEffect = FireComet_E;
      break;
  }
  
}

void iOsParticleTestApp::mouseDrag (MouseEvent event)
{
  switch (m_currentEffect)
  {
    case FireComet_E:
      m_fireComet->setPosition (vec3 ((float)event.getX(), (float)event.getY(), 0.f));
      break;
      
    case Flame_E:
      m_flame->setPosition (vec3 ((float)event.getX(), (float)event.getY(), 0.f));
      break;
  }
}

void iOsParticleTestApp::update()
{
  ParticleSystemManager::getSingleton().update();
}

void iOsParticleTestApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) );
  
  gl::setMatricesWindowPersp (getWindowWidth(), getWindowHeight(), 60.f, 10.f, 10000.f, true);
  
  // Draw particle system
  gl::disableDepthWrite ();
  
  gl::drawSphere(vec3(0,0,0), 1.f);
  
  ParticleSystemManager::getSingleton ().draw ();
  
  /*
  gl::ScopedModelMatrix modelScope;
  gl::rotate( M_PI/2 );
	gl::translate( 0, -getWindowWidth() );
  
	Rectf flippedBounds( 0, 0, getWindowHeight(), getWindowWidth() );
	gl::draw (gl::Texture::create (*cameraDist->getImage()), flippedBounds);
  */
  
  static uint32_t frameCount = 0UL;
  if (frameCount % 100 == 0 || (getAverageFps() < 30.f && frameCount % 10 == 0) || getAverageFps() < 5.f)
    CI_LOG_V( "fps: " << getAverageFps() << " particle count: " << ParticleSystemManager::getSingleton().getCount());
  frameCount++;
}

//CINDER_APP( iOsParticleTestApp, RendererGl )
CINDER_APP (iOsParticleTestApp, RendererGl, [] (App::Settings *settings)
{
  //settings->setWindowSize( 1334, 750 );
  settings->setMultiTouchEnabled (false);
}
)


