#include "PhongMaterial.h"

#include "MiscMath.h"

const int colorMapIndex  = 0;
const int normalMapIndex = 1;

PhongMaterial::PhongMaterial (      gl::GlslProg  shader,
                              const ColorAf&      matAmbient,
                              const ColorAf&      matDiffuse,
                              const ColorAf&      matSpecular,
                              const float         matShininess)
{
  mMatAmbient[0] = matAmbient.r;
  mMatAmbient[1] = matAmbient.g;
  mMatAmbient[2] = matAmbient.b;
  mMatAmbient[3] = matAmbient.a;

  mMatDiffuse[0] = matDiffuse.r;
  mMatDiffuse[1] = matDiffuse.g;
  mMatDiffuse[2] = matDiffuse.b;
  mMatDiffuse[3] = matDiffuse.a;

  mMatSpecular[0] = matSpecular.r;
  mMatSpecular[1] = matSpecular.g;
  mMatSpecular[2] = matSpecular.b;
  mMatSpecular[3] = matSpecular.a;

  mMatShininess = matShininess;

  mShader         = shader;
}

void PhongMaterial::bind ()
{
	glDisable (GL_BLEND);

  // Setup material for planets
  glMaterialfv (GL_FRONT, GL_AMBIENT,	   mMatAmbient);
  glMaterialfv (GL_FRONT, GL_DIFFUSE,	   mMatDiffuse);
  glMaterialfv (GL_FRONT, GL_SPECULAR,   mMatSpecular);
  glMaterialfv (GL_FRONT, GL_SHININESS, &mMatShininess);

	mShader.bind();
}

void PhongMaterial::unbind ()
{
	mShader.unbind();
}
