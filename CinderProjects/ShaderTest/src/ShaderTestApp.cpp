#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include "cinder/gl/Vbo.h"
#include "cinder/ObjLoader.h"

#include "ShaderHelper.h"
#include "PhongMaterial.h"
#include "MovingCamera.h"
#include "Macros.h"
#include "Miscellaneous.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class ShaderTestApp : public AppBasic 
{
public:
  void prepareSettings (Settings *settings);
	void setup();
  void keyDown (KeyEvent event);
  void mouseMove (MouseEvent event);
	void update();
	void draw();

private:
  shared_ptr<MovingCamera>  mCamera;
  shared_ptr<PhongMaterial> mMaterial;

  bool mPaused;
  bool mWireFrameMode;

  uint32_t frameCount;

  std::vector<Vec3f> points;
};

void ShaderTestApp::prepareSettings (Settings *settings)
{
  settings->setWindowSize (1600, 1000);
}

void ShaderTestApp::setup()
{
  float white[] = {1.0f, 1.0f, 1.0f , 0.0f};
  float red[]   = {1.0f, 0.0f, 0.0f , 0.0f};
  float green[] = {0.2f, 0.8f, 0.3f , 0.0f};
  float blue[]  = {0.0f, 0.0f, 1.0f , 0.0f};
  float black[] = {0.0f, 0.0f, 0.0f , 0.0f};

  float p1[] = {-1000, 5000, 0};

  glLightfv(GL_LIGHT0, GL_SPECULAR, white);
  glLightfv(GL_LIGHT0, GL_DIFFUSE,  white);
  glLightfv(GL_LIGHT0, GL_AMBIENT,  white);
	glLightfv(GL_LIGHT0, GL_POSITION, p1);
	glEnable (GL_LIGHT0);

  gl::enableDepthRead ();
  gl::enableDepthWrite ();

  gl::GlslProg shader = ShaderHelper::loadShader ("../Media/Shaders/tube_vert.glsl", 
                                                  "../Media/Shaders/tube_frag.glsl",
                                                  "../Media/Shaders/tube_geom.glsl",
                                                  GL_POINTS,
                                                  GL_TRIANGLE_STRIP,
                                                  1024);

  mMaterial.reset (new PhongMaterial (shader,
                                      ColorAf (0.05f, 0.1f,  0.05f, 1.0f),  // matAmbient,
                                      ColorAf (0.8f,  0.5f,  0.2f,  1.0f),  // matDiffuse,
                                      ColorAf (1.0f,  0.7f,  0.6f,  1.0f),  // matSpecular,
                                      10.0f));                               // matShininess                         // matShininess

  mCamera.reset (new MovingCamera(30.0f, 1.0f));


  frameCount     = 0;
  mWireFrameMode = false;
  mPaused        = false;
}


void ShaderTestApp::keyDown (KeyEvent event)
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

  mCamera->keyDown (event);
}


void ShaderTestApp::mouseMove (MouseEvent event)
{
  mCamera->mouseMove (event);
}

void ShaderTestApp::update()
{
  if (mPaused)
    return;

  points.clear ();

  points.push_back (Vec3f (0, 0, 0));
  points.push_back (Vec3f (3, 0, 0));
  for (uint32_t i=1; i<50; i++)
  {
    Vec3f prevDiff = points[i] - points[i-1];
    points.push_back (points[i] + prevDiff + Vec3f(Rand::randFloat (-3.f, 3.f),
                                                   Rand::randFloat (-3.f, 3.f),
                                                   Rand::randFloat (-3.f, 3.f)));
  }
}

// Projects v onto a plane with normal n
// n must be of unity length
Vec3f projectOnPlane (const Vec3f& n, const Vec3f& v)
{
  return v - n * n.dot (v);
}

void ShaderTestApp::draw()
{
	// clear out the window with black
	gl::clear (Color (0, 0, 0)); 

  // Setup camera
  mCamera->setMatrices ();

  // Choose a general direction for the "up" vector so that it is perpendicular to the 
  // general layout of the entire line, in that way the face normals are less likely 
  // to be aligned to the direction (which is not good when projecting on that plane)
  Vec3f upDirection = Vec3f (0,0,1).cross (points[points.size()-1] - points[0]);

  Vec3f planeNormal1;
  Vec3f planeNormal2;

#ifdef DEBUG
  Vec3f up1;
  Vec3f up2;

  Vec3f side1;
  Vec3f side2;
#else
  mMaterial->bind ();
#endif

/*
  gl::translate (Vec3f (-200.f*3.f/2.f, 0, 0));
  for (uint32_t x = 0; x < 200; x++)
  {
    gl::translate (Vec3f (3, 0, 0));
*/
  for (uint32_t i=0; i<points.size() - 2; i++)
  {
    Vec3f currentToNext  = points[i+1] - points[i];
    Vec3f nextToNextNext = points[i+2] - points[i+1];

    if (i==0)
      planeNormal1 = currentToNext.normalized ();
    else
      planeNormal1 = planeNormal2;

    planeNormal2 = (currentToNext + nextToNextNext).normalized ();

#ifdef DEBUG
    if (i==0)
    {
      up1          = projectOnPlane (planeNormal1, upDirection).normalized ();
      side1        = planeNormal1.cross (up1).normalized();
    }
    else
    {
      up1          = up2;
      side1        = side2;
    }

    up2          = projectOnPlane (planeNormal2, upDirection).normalized ();
    side2        = planeNormal2.cross (up2).normalized();

    gl::color (1,0,0);
    gl::drawVector (points[i]*0.5f, (points[i] + planeNormal1)*0.5f);
    gl::color (0,1,0);
    gl::drawVector (points[i]*0.5f, (points[i] + up1)*0.5f);
    gl::color (0,0,1);
    gl::drawVector (points[i]*0.5f, (points[i] + side1)*0.5f);

    mMaterial->bind ();
#endif


    mMaterial->getShader().uniform ("u_generalUp",    upDirection);

    mMaterial->getShader().uniform ("u_point2",       points[i+1]);
    mMaterial->getShader().uniform ("u_planeNormal1", planeNormal1);
    mMaterial->getShader().uniform ("u_planeNormal2", planeNormal2);

    gl::begin (GL_POINTS);
    gl::vertex (points[i]);
    gl::end ();

#ifdef DEBUG
    mMaterial->unbind ();
#endif

  }
//  }

#ifndef DEBUG
    mMaterial->unbind ();
#endif

  Misc::CheckForOpenGlError ();

  frameCount++;
  if ((frameCount % 10) == 0)
    console() << "FPS: " << getAverageFps() << std::endl;
}

CINDER_APP_BASIC( ShaderTestApp, RendererGl )
