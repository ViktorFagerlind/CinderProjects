#include "BloomEffect.h"


BloomEffect::BloomEffect (unsigned int blurWidth, 
                          unsigned int blurHeight, 
                          unsigned int screenWidth, 
                          unsigned int screenHeight)
: mBlurSize (1.0f)
{
  mFinalFbo          = gl::Fbo (screenWidth,  screenHeight, true, true, true);
  mHorizontalBlurFbo = gl::Fbo (blurWidth,    blurHeight,   true, true, true);
  mBothBlurFbo       = gl::Fbo (blurWidth,    blurHeight,   true, true, true);

  // Load blur shader
	try {
		mBlurShader = gl::GlslProg (loadFile ("../Media/Shaders/passThru_vert.glsl"), loadFile ("../Media/Shaders/blur_frag.glsl"));
	}	catch (gl::GlslProgCompileExc &exc) {
		std::cout << "Shader compile error: " << std::endl;
		std::cout << exc.what();
	}	catch (...) {
		std::cout << "Unable to load shader" << std::endl;
	}
}


BloomEffect::~BloomEffect ()
{
}

void BloomEffect::blurStep (gl::Fbo& fromFbo, gl::Fbo& toFbo, const Vec2f& blur)
{
  gl::setMatricesWindow (toFbo.getSize (), false);
  toFbo.bindFramebuffer ();

	mBlurShader.uniform("pixelOffset", blur);

	gl::draw (fromFbo.getTexture(), fromFbo.getTexture().getBounds(), toFbo.getBounds());

  toFbo.unbindFramebuffer ();
}


gl::Fbo& BloomEffect::render (gl::Fbo& original)
{
  // Do not test for depth when drawing FBOs
  gl::disableDepthRead ();
  gl::color (1, 1, 1, 1);

  mBlurShader.bind ();
	mBlurShader.uniform("tex0", 0); // use texture unit 0

	// draw horizontal blur
  blurStep (original, mHorizontalBlurFbo, Vec2f(mBlurSize/mHorizontalBlurFbo.getWidth (), 0.0f));

	// draw vertical blur (of horizontal blur)
  blurStep (mHorizontalBlurFbo, mBothBlurFbo, Vec2f(0.0f, mBlurSize/mBothBlurFbo.getHeight ()));

  mBlurShader.unbind ();

  return mBothBlurFbo;

/*
  gl::setMatricesWindow (mFinalFbo.getSize (), false);
  mFinalFbo.bindFramebuffer ();
	gl::draw (mBothBlurFbo.getTexture(), mBothBlurFbo.getTexture().getBounds(), mFinalFbo.getBounds());
  mFinalFbo.unbindFramebuffer ();

  return mFinalFbo;
*/
}
