#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Log.h"

#include "Macros.h"
#include "ParticleSystemManager.h"
#include "ParticleSystem.h"
#include "ImageLibrary.h"
#include "Emitter.h"
#include "AreaEmitter.h"
#include "PointEmitter.h"
#include "CommonModifier.h"
#include "ColorModifier.h"
#include "PerlinModifier.h"
#include "DampingModifier.h"


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
  vector<shared_ptr<Emitter>>  m_emitters;
};

void iOsParticleTestApp::setup()
{
  // Camera
//  m_cam.setAspectRatio ((float)getWindowWidth() / (float)getWindowHeight());
//  m_cam.lookAt (vec3 ((float)getWindowWidth()/2.f, (float)getWindowHeight()/2.f, getWindowWidth()),
//                vec3 ((float)getWindowWidth()/2.f, (float)getWindowHeight()/2.f, 0.f));
  
  
  ParticleSystem  *particleSystem;
  CommonModifier  *commonModifier;
  ColorModifier   *colorModifier;
  PerlinModifier  *perlinModifier;
  DampingModifier *dampingModifier;
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////////
  //  Fire
  ///////////
  particleSystem = new ParticleSystem (ImageLibrary::getSingleton ().getTexture ("fire_alpha.png"));
  
  commonModifier = new CommonModifier (2.5f, 1.0f, 2.0f);
  colorModifier  = new ColorModifier  (ColorAf(0.8f,  0.8f,  1.0f,  1.0f),  //startColor
                                       ColorAf(1.0f,  0.2f,  0.2f,  0.3f),  //middleColor
                                       ColorAf(1.0f,  0.2f,  0.2f,  0.0f),  //endColor
                                       0.7f);                               //middleTime
  dampingModifier = new DampingModifier (0.025f);
  
  particleSystem->addModifier (commonModifier);
  particleSystem->addModifier (colorModifier);
  particleSystem->addModifier (dampingModifier);
  
  m_emitters.push_back ((shared_ptr<Emitter>) new PointEmitter (700,                 // maxNofParticles,
                                     vec3((float)getWindowWidth()/2.f, (float)getWindowHeight()/2.f, 0.f), // position,
                                     15.f,                 // particlesPerFrame,
                                     60.f,                // minParticleSize,
                                     80.f,                // maxParticleSize,
                                     vec3(0),             // baseVelocity,
                                     4.f,                 // minRandVelocity
                                     5.f));                // maxRandVelocity
  
  particleSystem->addEmitterRef (m_emitters.back());
  
  ParticleSystemManager::getSingleton().addParticleSystem (particleSystem);
  
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////////
  //  Sparks
  ///////////
  particleSystem = new ParticleSystem (ImageLibrary::getSingleton ().getTexture ("flare.png"));
  
  commonModifier = new CommonModifier (1.5f, 1.0f, 0.8f);
  colorModifier  = new ColorModifier  (ColorAf(1.0f,  1.0f,  1.0f,  1.0f),  //startColor
                                       ColorAf(1.0f,  1.0f,  0.2f,  1.0f),  //middleColor
                                       ColorAf(1.0f,  0.2f,  0.2f,  0.0f),  //endColor
                                       0.8f);                               //middleTime
  
  particleSystem->addModifier (commonModifier);
  particleSystem->addModifier (colorModifier);
  
  m_emitters.push_back ((shared_ptr<Emitter>) new PointEmitter (100,                 // maxNofParticles,
                                                                vec3((float)getWindowWidth()/2.f, (float)getWindowHeight()/2.f, 0.f), // position,
                                                                .5f,                 // particlesPerFrame,
                                                                10.f,                // minParticleSize,
                                                                20.f,                // maxParticleSize,
                                                                vec3(0),             // baseVelocity,
                                                                1.f,                 // minRandVelocity
                                                                2.f));                // maxRandVelocity
  
  particleSystem->addEmitterRef (m_emitters.back());
  
  ParticleSystemManager::getSingleton().addParticleSystem (particleSystem);
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////////
  //  Smoke
  ///////////
  particleSystem = new ParticleSystem (ImageLibrary::getSingleton ().getTexture ("smoke1.jpg"));
  
  commonModifier = new CommonModifier (.8f, 1.0f, 5.0f);
  colorModifier  = new ColorModifier  (ColorAf(1.0f,  1.0f,  1.0f,  0.04f),  //startColor
                                       ColorAf(1.0f,  1.0f,  1.0f,  0.07f),  //middleColor
                                       ColorAf(1.0f,  1.0f,  1.0f,  0.0f),  //endColor
                                       0.5f);                               //middleTime
  perlinModifier = new PerlinModifier (10.0f, 0.1, 0.003, 0.002f);
  
  particleSystem->addModifier (commonModifier);
  particleSystem->addModifier (colorModifier);
  particleSystem->addModifier (perlinModifier);
  
  m_emitters.push_back ((shared_ptr<Emitter>) new AreaEmitter (1200,                 // maxNofParticles,
                                     vec3((float)getWindowWidth()/2.f, (float)getWindowHeight()/2.f, 0.f), // position,
                                     3.f,                 // particlesPerFrame,
                                     50.f,  // width,
                                     50.f, 	// height,
                                     0.f,   // depth
                                     50.f,                // minParticleSize,
                                     100.f,                // maxParticleSize,
                                     vec3(0,-2.5f,0),             // baseVelocity,
                                     3.0f));                // maxRandVelocity
  
  particleSystem->addEmitterRef (m_emitters.back());
  
  ParticleSystemManager::getSingleton().addParticleSystem (particleSystem);
  
}

void iOsParticleTestApp::mouseDown (MouseEvent event)
{
  mouseDrag (event);
}

void iOsParticleTestApp::mouseDrag (MouseEvent event)
{
  for (const auto e : m_emitters)
    e->setPosition(vec3 ((float)event.getX(), (float)event.getY(), 0.f));
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
  gl::enableAdditiveBlending ();
  
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

