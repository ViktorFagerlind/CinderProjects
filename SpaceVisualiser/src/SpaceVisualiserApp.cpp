#include "VfBaseApp.h"

#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"
#include "cinder/Rand.h"
#include "cinder/gl/Vbo.h"
#include "cinder/ObjLoader.h"

/*
#include "cinder/audio/Context.h"
#include "cinder/audio/NodeEffects.h"
#include "cinder/audio/SamplePlayerNode.h"
#include "cinder/audio/dsp/Fft.h"
*/


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


#include "FMOD.hpp"

// Channel callback function used by FMOD to notify us of channel events
FMOD_RESULT F_CALLBACK channelCallback (FMOD_CHANNEL *channel, FMOD_CHANNEL_CALLBACKTYPE type, void *commanddata1, void *commanddata2);

using namespace ci;
using namespace ci::app;
using namespace std;


class VisualChannel
{
public:
  VisualChannel (uint32_t nofBands, uint32_t nofHistory)
  {
    m_nofBands        = nofBands;
    m_nofHistory      = nofHistory;
    m_previousIndex   = 0;
    m_samples         = 0;

    m_data            = new float[m_nofBands * m_nofHistory];
    m_bandAverages    = new float[m_nofBands];
    m_bandAccAverages = new float[m_nofBands];

    memset (m_data,             0, sizeof (float) * m_nofBands * m_nofHistory);
    memset (m_bandAverages,     0, sizeof (float) * m_nofBands);
    memset (m_bandAccAverages,  0, sizeof (float) * m_nofBands);
  }

  float getRaw (uint32_t band)      const { return m_data[m_previousIndex*m_nofBands + band]; }
  float getAvg (uint32_t band)      const { return m_bandAverages[band]; }
  float getAccAvg (uint32_t band)   const { return m_bandAccAverages[band]; }

  float getRelAvg (uint32_t band)   const { return getAvg (band) / (getAccAvg (band) + 0.001f); }

  float getTotal ()                 const { return m_total; }
  float getTotalAvg ()              const { return m_totalAverage; }

  void addSample (float *data)
  { 
    float avgSum = 0.f;
    float nowSum = 0.f;

    m_samples++;

    uint32_t nextIndex = m_samples % m_nofHistory;

    memcpy (&m_data[nextIndex*m_nofBands], data, m_nofBands);

    for (uint32_t i = 0; i < m_nofBands; i++)
    {
      float bandSum = 0.f;
      for (uint32_t j = 0; j < m_nofHistory; j++)
        bandSum += m_data[j*m_nofBands + i];

      nowSum += data[i];

      m_bandAverages[i] = bandSum / (float)m_nofHistory;
      avgSum += m_bandAverages[i];

      m_bandAccAverages[i] = (m_bandAccAverages[i] * (m_samples - 1) + data[i]) / m_samples;

    }
    m_total = nowSum / (float)m_nofBands;
    m_totalAverage = avgSum / (float)m_nofBands;

    m_previousIndex = nextIndex;
  }

private:

  
  float *m_data;              // Contains the raw data for each band with stored history
  float *m_bandAverages;      // Contains the average from the stored history for each band
  float *m_bandAccAverages;   // Contains the average from the the entire history from start for each band


  float  m_total;
  float  m_totalAverage;
  float  m_totalAccAverage;

  uint32_t m_nofBands;
  uint32_t m_nofHistory;
  uint32_t m_previousIndex;

  uint32_t m_samples;
};

class SpaceVisualiserApp : public VfBaseApp 
{
public:
  void setup ();
  void setupAudio ();
  void setupFMOD ();
  void shutdown ();

  void keyDown (KeyEvent event);
  void mouseDown (MouseEvent event);

	void update();

  void drawBands (bool isLeft, VisualChannel &ch);
  void drawBands ();
  void draw ();

private:
  std::vector<shared_ptr<Star>> m_Stars;

  shared_ptr<BloomEffect>       m_bloomEffect;
  /*
  audio::BufferPlayerNodeRef		m_player;
  audio::GainNodeRef				    m_gain;
  */

  FMOD::System*		m_FMODSystem;
  FMOD::Sound*		m_FMODSound;
  FMOD::Channel*	m_FMODChannel;

  const uint32_t  m_nofBands   = 64;
  const uint32_t  m_nofHistory = 8;

  float *m_captureDataLeft;
  float *m_captureDataRight;

  shared_ptr<VisualChannel> m_channelLeft;
  shared_ptr<VisualChannel> m_channelRight;
};

// Channel callback function used by FMOD to notify us of channel events
FMOD_RESULT F_CALLBACK channelCallback (FMOD_CHANNEL *channel, FMOD_CHANNEL_CALLBACKTYPE type, void *commanddata1, void *commanddata2)
{
  /*
  SpaceVisualiserApp *app = static_cast<SpaceVisualiserApp*>(App::get ());

  // now handle the callback
  switch (type)
  {
  case FMOD_CHANNEL_CALLBACKTYPE_END:
    break;
  default:
    break;
  }
  */

  return FMOD_OK;
}

void SpaceVisualiserApp::setupFMOD ()
{
  FMOD_RESULT err;

  // play audio using the Cinder FMOD block
  FMOD::System_Create (&m_FMODSystem);
  m_FMODSystem->init (32, FMOD_INIT_NORMAL | FMOD_INIT_ENABLE_PROFILE, NULL);
  m_FMODSound = nullptr;
  m_FMODChannel = nullptr;

  // stream the audio
  err = m_FMODSystem->createStream ("../Media/Audio/Gin & Tony - End Of The Old World.mp3", FMOD_SOFTWARE, NULL, &m_FMODSound);
  err = m_FMODSystem->playSound (FMOD_CHANNEL_FREE, m_FMODSound, false, &m_FMODChannel);

  // we want to be notified of channel events
  err = m_FMODChannel->setCallback (channelCallback);

  // create channels from which we can construct our textures
  m_channelLeft.reset (new VisualChannel (m_nofBands, m_nofHistory));
  m_channelRight.reset (new VisualChannel (m_nofBands, m_nofHistory));

  m_captureDataLeft  = new float[m_nofBands];
  m_captureDataRight = new float[m_nofBands];
}

void SpaceVisualiserApp::setupAudio ()
{
  /*
  auto ctx = audio::Context::master ();

  audio::SourceFileRef sourceFile = audio::load (loadFile ("../Media/Audio/Gin & Tony - End Of The Old World.mp3"), ctx->getSampleRate ());

  // load the entire sound file into a BufferRef, and construct a BufferPlayerNode with this.
  audio::BufferRef buffer = sourceFile->loadBuffer ();
  m_player = ctx->makeNode (new audio::BufferPlayerNode (buffer));

  // add a Gain to reduce the volume
  m_gain = ctx->makeNode (new audio::GainNode (1.0f));

  // connect and enable the Context
  m_player >> m_gain >> ctx->getOutput ();
  ctx->enable ();

  m_player->start ();
  */
}

void SpaceVisualiserApp::setup ()
{
  gl::Fbo::Format format;
  format.setSamples (16); // 8x antialiasing
  format.setColorInternalFormat (GL_RGBA32F);

  VfBaseApp::setup (130.0f, 1.0f, format);

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


  for (uint32_t i=0; i<5; i++)
  {
    shared_ptr<Star> Star (new Star (particleSystem,                      // System
                                     Rand::randFloat (4.f, 6.f),          // Size
                                     vec3(Rand::randFloat(-50,50),     // Position
                                           Rand::randFloat(-50,50),
                                           Rand::randFloat(-50,50))));

    m_Stars.push_back (Star);
  }

  m_bloomEffect.reset (new BloomEffect (getWindowWidth()/4, getWindowHeight()/4, getWindowWidth(), getWindowHeight()));

  //setupAudio ();
  setupFMOD ();


  setFrameRate (30.f);
//  hideCursor ();
}

void SpaceVisualiserApp::shutdown ()
{
  // properly shut down FMOD
  FMOD_RESULT err;

  if (m_FMODChannel == nullptr || m_FMODSound == nullptr)
    return;

  // we don't want to be notified of channel events any longer
  m_FMODChannel->setCallback (nullptr);

  bool isPlaying;
  err = m_FMODChannel->isPlaying (&isPlaying);
  if (isPlaying)
    err = m_FMODChannel->stop ();

  err = m_FMODSound->release ();

  m_FMODSound    = nullptr;
  m_FMODChannel  = nullptr;

  if (m_FMODSystem)
    m_FMODSystem->release ();

  delete[] m_captureDataLeft;
  delete[] m_captureDataRight;
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
      format.setColorInternalFormat (GL_RGBA32F);
      m_frameBuffer.reset (new gl::Fbo (getWindowWidth (), getWindowHeight (), format));
      m_bloomEffect.reset (new BloomEffect (getWindowWidth()/4, getWindowHeight()/4, getWindowWidth(), getWindowHeight()));
      break;
    }  


  }

}


void SpaceVisualiserApp::update()
{
  // update FMOD so it can notify us of events
  m_FMODSystem->update ();

  // get spectrum for left and right channels and copy it into our channels
  m_FMODSystem->getSpectrum (m_captureDataLeft,  m_nofBands, 0, FMOD_DSP_FFT_WINDOW_HANNING);
  m_FMODSystem->getSpectrum (m_captureDataRight, m_nofBands, 1, FMOD_DSP_FFT_WINDOW_HANNING);

  m_channelLeft->addSample  (m_captureDataLeft);
  m_channelRight->addSample (m_captureDataRight);

  if (m_paused)
    return;

  vector<float> bands;
  bands.push_back (m_channelLeft->getRelAvg (0));
  bands.push_back (m_channelLeft->getRelAvg (4));
  bands.push_back (m_channelLeft->getRelAvg (8));

  for (uint32_t i = 0; i < m_Stars.size (); i++)
  {
    m_Stars[i]->update ();

    m_Stars[i]->setBands (bands);
  }

  ParticleSystemManager::getSingleton ().update ();
}

void SpaceVisualiserApp::drawBands (bool isLeft, VisualChannel &ch)
{
  const float mult   = isLeft ? -1.f : 1.f;
  const float width  = 1.f;
  const float height = 200.f;

  for (uint32_t i = 0; i < m_nofBands; i++)
  {
    gl::color (1, (float)i / (float)m_nofBands, 0);

    vec3 pos   = vec3 (mult * (100.f - (float)i * width), 0.f, 0);
    vec3 size  = vec3 (width, 0.f, 1);

    pos.y = -40.f; size.y = height * ch.getRaw (i);
    gl::drawCube (pos, size);

    pos.y = 0.f; size.y = height * ch.getAvg (i);
    gl::drawCube (pos, size);

    pos.y = 40.f; size.y = height * ch.getRelAvg (i) * 0.01f;
    gl::drawCube (pos, size);
  }
}


void SpaceVisualiserApp::drawBands ()
{
  drawBands (true,  *m_channelLeft.get ());
  drawBands (false, *m_channelRight.get ());
}


void SpaceVisualiserApp::draw ()
{
  // Draw to frame buffer from now on
  m_frameBuffer->bindFramebuffer();

	// clear the window with black
  gl::clear (Color::black ());

  gl::enableDepthRead ();
  gl::enableDepthWrite ();

  if (m_wireFrameMode)
    gl::enableWireframe ();

  // Setup camera
  mCamera->setMatrices ();

  //drawBands ();

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
