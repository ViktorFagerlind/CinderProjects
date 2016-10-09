#include "AnimParticleDrawer.h"

AnimParticleDrawer::AnimParticleDrawer (const string& vertexShaderName,
                                        const string& fragmentShaderName)
: NewParticleDrawer (vertexShaderName, fragmentShaderName)
{
}

AnimParticleDrawer::~AnimParticleDrawer ()
{
}

void AnimParticleDrawer::beforeDraw ()
{
  /*
   uniform float ImageSize;      // Size of the individual images (in the sprite sheet)
   uniform float NofWidthImages; // The number of images for each row
   uniform float TotalNofImages; // The total number of images
   */
  
  m_particleBatch->getGlslProg ()->uniform ("ImageSize", 7);
  m_particleBatch->getGlslProg ()->uniform ("NofWidthImages", 146);
  m_particleBatch->getGlslProg ()->uniform ("TotalNofImages", 89);
  
  NewParticleDrawer::beforeDraw();
}

/*
void AnimParticleDrawer::drawParticle (const Particle &p, const vec2 &textureSize)
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
                               vec2 (p.mCurrentSize, p.mCurrentSize),
                               texCoords,
                               p.mColor);
  
}
*/
