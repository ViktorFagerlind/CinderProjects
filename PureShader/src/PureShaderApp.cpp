#include "VfBaseApp.h"

#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/camera.h"

#include "ShaderHelper.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class PureShaderApp : public VfBaseApp 
{
public:
	void setup();
  void keyDown (KeyEvent event);
  void mouseDown (MouseEvent event);
	void update ();
	void draw ();

private:
  gl::GlslProg m_shader;

  CameraOrtho  m_camera;

  float        m_imageRatio;
};

void PureShaderApp::setup()
{
  VfBaseApp::setup (80.0f, 1.0f);

  m_shader = ShaderHelper::loadShader("../Media/Shaders/passThru_vert.glsl", 
                                      "../Media/Shaders/pure_frag.glsl");

  m_imageRatio = (float)getWindowWidth () / (float)getWindowHeight ();

  m_camera.setOrtho (-m_imageRatio, m_imageRatio, 
                     -1,            1, 
                     -1,            1);

}

void PureShaderApp::mouseDown (MouseEvent event)
{
}

void PureShaderApp::keyDown (KeyEvent event)
{
}

void PureShaderApp::update()
{
}

void PureShaderApp::draw()
{
  // Draw to frame buffer from now on
  m_frameBuffer->bindFramebuffer();

  gl::setMatrices (m_camera);

  m_shader.bind ();

  m_shader.uniform ("iResolution", Vec3f ((float)getWindowWidth (), (float)getWindowHeight (), 0.f));
  m_shader.uniform ("iGlobalTime", (float)m_frameCount / 60.f);

  gl::begin (GL_TRIANGLE_STRIP);

  gl::vertex (-m_imageRatio,  1, 0);
  gl::vertex ( m_imageRatio,  1, 0);
  gl::vertex (-m_imageRatio, -1, 0);
  gl::vertex ( m_imageRatio, -1, 0);

  gl::end ();
  m_shader.unbind ();

  m_frameBuffer->unbindFramebuffer();

  VfBaseApp::drawToScreen ();
}



CINDER_APP_BASIC( PureShaderApp, RendererGl )
