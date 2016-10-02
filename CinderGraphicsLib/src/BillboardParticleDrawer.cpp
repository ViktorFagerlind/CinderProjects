#include "BillboardParticleDrawer.h"

#include "Billboard.h"

BillboardParticleDrawer::BillboardParticleDrawer ()
{
}

BillboardParticleDrawer::~BillboardParticleDrawer ()
{
}

void BillboardParticleDrawer::beforeDraw ()
{
  // Enable arrays
  glEnableClientState (GL_VERTEX_ARRAY);
  glEnableClientState (GL_TEXTURE_COORD_ARRAY);
  glEnableClientState (GL_COLOR_ARRAY);
}

void BillboardParticleDrawer::drawParticle (const Particle &p, const vec2 &textureSize)
{
  Billboard::drawBillboard (p.mPosition, vec2 (p.mCurrentSize, p.mCurrentSize), p.mColor);
}

void BillboardParticleDrawer::afterDraw ()
{
  // Disable arrays
  glDisableClientState (GL_VERTEX_ARRAY);
  glDisableClientState (GL_TEXTURE_COORD_ARRAY);
  glDisableClientState (GL_COLOR_ARRAY);
}
