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

  ColorAf getDiffuse ()  { return ColorAf (mMatDiffuse[0], mMatDiffuse[1], mMatDiffuse[2], mMatDiffuse[3]); }

  void setAmbient (const ColorAf& c)  { mMatAmbient[0]  = c.r; mMatAmbient[1]  = c.g; mMatAmbient[2]  = c.b; mMatAmbient[3]  = c.a; }
  void setDiffuse (const ColorAf& c)  { mMatDiffuse[0]  = c.r; mMatDiffuse[1]  = c.g; mMatDiffuse[2]  = c.b; mMatDiffuse[3]  = c.a; }
  void setSpecular (const ColorAf& c) { mMatSpecular[0] = c.r; mMatSpecular[1] = c.g; mMatSpecular[2] = c.b; mMatSpecular[3] = c.a; }
  void setShininess (const float s)   { mMatShininess = s; }

  gl::GlslProg& getShader () {return mShader;}

private:
	gl::GlslProg mShader;

  float mMatAmbient[4];
  float mMatDiffuse[4];
  float mMatSpecular[4];
  float mMatShininess;
};


