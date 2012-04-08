#pragma once

#include "cinder/gl/Fbo.h"
#include "cinder/gl/GlslProg.h"

using namespace ci;
using namespace std;

class BloomEffect
{
public:
  BloomEffect (unsigned int blurWidth, 
               unsigned int blurHeight, 
               unsigned int screenWidth, 
               unsigned int screenHeight);

  ~BloomEffect ();

  gl::Fbo& render (gl::Fbo& original);

private:
  void blurStep (gl::Fbo& fromFbo, gl::Fbo& toFbo, const Vec2f& blur);

private:
  const float mBlurSize;

  gl::Fbo mFinalFbo;
  gl::Fbo mHorizontalBlurFbo;
  gl::Fbo mBothBlurFbo;

  gl::GlslProg  mBlurShader;
};

