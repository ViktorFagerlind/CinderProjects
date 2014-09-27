#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "cinder/Easing.h"
#include "cinder/CinderResources.h"

#include "SpriteSheet.h"
#include <stdio.h>

using namespace ci;
using namespace ci::app;
using namespace std;

class TextureAnimationApp : public AppBasic {
  public:
	void setup();
	void update();
	void draw();

	gl::Texture backgroundTexture;
	gl::Texture bunnyWalkTexture;
	SpriteSheet bunnyWalk;

};

void TextureAnimationApp::setup()
{
	bunnyWalkTexture = loadImage (loadFile ("../Media/Movies/InkParticle/InkParticle.png"));
	bunnyWalk = SpriteSheet();
	bunnyWalk.init (bunnyWalkTexture, 
                  "../Media/Movies/InkParticle/InkParticle.xml", 
                  SpriteSheet::FORMAT_TEXTUREPACKER_GENERIC_XML);

	bunnyWalk.x = 300.0f;
	bunnyWalk.y = 100.0f;

	gl::enableAlphaBlending();

//	timeline().apply (&bunnyWalk.x, 600.0f,-200.0f,5.0f,ci::EaseNone()).loop(true);
//	timeline().apply (&bunnyWalk.alpha, 0.0f,1.0f,5.0f,ci::EaseNone()).loop(true);
}


void TextureAnimationApp::update()
{
	bunnyWalk.update();
}

void TextureAnimationApp::draw()
{
	// clear the window
	gl::clear( Color( 0.0, 0.0, 0.0 ) );
	
	bunnyWalk.draw();
}

CINDER_APP_BASIC( TextureAnimationApp, RendererGl )
