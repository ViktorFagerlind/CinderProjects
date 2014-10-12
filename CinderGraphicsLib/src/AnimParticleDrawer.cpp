#include "AnimParticleDrawer.h"

#include "Billboard.h"


AnimParticleDrawer::AnimParticleDrawer (shared_ptr<vector<SpriteData>> spriteData)
{
  m_spriteData = spriteData;
}

AnimParticleDrawer::~AnimParticleDrawer ()
{
}

void AnimParticleDrawer::beforeDraw ()
{
  // Enable arrays
  glEnableClientState (GL_VERTEX_ARRAY);
  glEnableClientState (GL_TEXTURE_COORD_ARRAY);
  glEnableClientState (GL_COLOR_ARRAY);
}

void AnimParticleDrawer::drawParticle (const Particle &p, const Vec2f &textureSize)
{
  GLfloat texCoords[8];

  uint32_t animationNofFrames = m_spriteData->size ();

  // Calculate current frame --------------
  uint32_t frameNr = (uint32_t)((1.0f - (p.getLife () / Particle_fullLife_C)) * animationNofFrames);
  if (frameNr >= animationNofFrames)
    frameNr = animationNofFrames - 1;

  SpriteData *frame = &m_spriteData->operator[] (frameNr);

  float xl = (float)frame->x / textureSize.x;
  float xr = (float)(frame->x + frame->w) / textureSize.x;
  float yu = (float)frame->y / textureSize.y;
  float yd = (float)(frame->y + frame->h) / textureSize.y;

  // Calculate texture coordinates --------------
  // top left
  texCoords[0] = xl;
  texCoords[1] = yu;
  // top right
  texCoords[2] = xl;
  texCoords[3] = yd;
  // bottom right
  texCoords[4] = xr;
  texCoords[5] = yu;
  // bottom left
  texCoords[6] = xr;
  texCoords[7] = yd;

  Billboard::drawBillboardTex (p.mPosition,
                               Vec2f (p.mCurrentSize, p.mCurrentSize),
                               texCoords,
                               p.mColor);
}

void AnimParticleDrawer::afterDraw ()
{
  // Disable arrays
  glDisableClientState (GL_VERTEX_ARRAY);
  glDisableClientState (GL_TEXTURE_COORD_ARRAY);
  glDisableClientState (GL_COLOR_ARRAY);
}
