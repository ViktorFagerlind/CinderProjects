#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/Camera.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class ShaderTestAppApp : public AppBasic 
{
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();

	// gl::Texture		mTexture;	
	gl::GlslProg	mShader;
	float			    mAngle;

	float			    mTime;

  CameraPersp mCam;
};


void ShaderTestAppApp::setup()
{
  /*
	try 
  {
		mTexture = gl::Texture( loadImage( loadFile("../Media/Images/football.png") ) );
	}
	catch (...) 
  {
		std::cout << "unable to load the texture file!" << std::endl;
	}
	*/

	try 
  {
		mShader = gl::GlslProg (loadFile ("../Media/Shaders/test1_vert.glsl"), loadFile ("../Media/Shaders/test1_frag.glsl"));
	}
	catch (gl::GlslProgCompileExc &exc) 
  {
		std::cout << "Shader compile error: " << std::endl;
		std::cout << exc.what();
	}
	catch (...) 
  {
		std::cout << "Unable to load shader" << std::endl;
	}
	
  mCam.setPerspective (60.0f, getWindowAspectRatio(), 100.0f, 5000.0f);
  Vec3f cameraPosition = Vec3f (0.0f, 0.0f, 1000.0f);
  mCam.lookAt(cameraPosition,  // camera
              Vec3f::zero(),   // center
              Vec3f::yAxis()); // up

  mAngle = 0.0f;
  mTime  = 0.0f;

  glEnable (GL_DEPTH_TEST);
}

void ShaderTestAppApp::mouseDown( MouseEvent event )
{
}

void ShaderTestAppApp::update()
{
	mAngle += 0.3f;

  mTime += 1.0f/60.0f;

//  if (mTime > 5.0f)
//    mTime = 0.0f;
}

void ShaderTestAppApp::draw()
{
	gl::clear();

	//mTexture.enableAndBind();

	mShader.bind();
//	mShader.uniform ("tex0", 0 );
//	mShader.uniform ("sampleOffset", Vec2f( cos( mAngle ), sin( mAngle ) ) * ( 1.0f / getWindowWidth() ));
	mShader.uniform ("t", mTime);

  gl::setMatrices (mCam);
  gl::rotate (Vec3f (mAngle, mAngle, mAngle));

//	gl::drawSolidRect( getWindowBounds() );

  gl::translate (Vec3f (-800, -800, 0));
  for (int y = 0; y < 4; y++)
  {
    for (int x = 0; x < 4; x++)
    {
	    gl::drawCube (Vec3f(0,0,0), Vec3f(150.0f, 150.0f, 150.0f));
      gl::drawSphere (Vec3f(0,0,0), 100.0f, 20);

      gl::translate (Vec3f (400, 0, 0));
    }
      gl::translate (Vec3f (0, 400, 0));
      gl::translate (Vec3f (-1600, 0, 0));
  }

//	mTexture.unbind();
}

CINDER_APP_BASIC( ShaderTestAppApp, RendererGl )
