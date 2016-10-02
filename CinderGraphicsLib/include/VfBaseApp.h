#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"

#include "MovingCamera.h"
#include "Macros.h"
#include "Miscellaneous.h"

#include "cinder/qtime/MovieWriter.h"
#include "cinder/ip/Flip.h"


using namespace ci;
using namespace ci::app;
using namespace std;

class VfBaseApp : public AppBasic 
{
public:
  virtual void prepareSettings (Settings *settings);
  virtual void keyDown (KeyEvent event);
  virtual void mouseMove (MouseEvent event);

  void setup (const float cameraDistance = 100.f, const float cameraStepSize = 1.f, gl::Fbo::Format fboFormat = gl::Fbo::Format ());
	void drawToScreen ();

protected:
  shared_ptr<MovingCamera> mCamera;

  bool    m_paused;
  bool    m_savingVideo;
  bool    m_wireFrameMode;

  unique_ptr<gl::Fbo> m_frameBuffer;

  qtime::MovieWriter	m_movieWriter;

  uint32_t            m_frameCount;
};
