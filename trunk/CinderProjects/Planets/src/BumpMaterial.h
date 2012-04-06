#pragma once

#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Texture.h"
#include "cinder/TriMesh.h"

#include <vector>

using namespace ci;
using namespace std;

class BumpMaterial
{
public:
  BumpMaterial (      gl::Texture   diffuseTexture,
                      gl::Texture   normalTexture,
                      gl::GlslProg  shader,
                const TriMesh&      mesh, 
                const ColorAf&      matAmbient,
                const ColorAf&      matDiffuse,
                const ColorAf&      matSpecular,
                const float         matShininess);

  void bind ();
  void unbind ();

private:
  void calculateTangents (const TriMesh& mesh);

private:
	gl::GlslProg mShader;
	gl::Texture  mDiffuseTexture;
	gl::Texture  mNormalTexture;

  vector<float> mTangentBuffer;

  float mMatAmbient[4];
  float mMatDiffuse[4];
  float mMatSpecular[4];
  float mMatShininess;

  GLint mTangentUniformLoc;
};


