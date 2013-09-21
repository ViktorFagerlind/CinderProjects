#pragma once

#include "BaseMaterial.h"

#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Texture.h"
#include "cinder/TriMesh.h"

#include <vector>

using namespace ci;
using namespace std;

class PhongMaterial : public BaseMaterial
{
public:
  PhongMaterial (      gl::GlslProg  shader,
                 const ColorAf&      matAmbient,
                 const ColorAf&      matDiffuse,
                 const ColorAf&      matSpecular,
                 const float         matShininess);

  void bind ();
  void unbind ();

  gl::GlslProg& getShader () {return mShader;}

private:
	gl::GlslProg mShader;

  float mMatAmbient[4];
  float mMatDiffuse[4];
  float mMatSpecular[4];
  float mMatShininess;
};

