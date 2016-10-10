#pragma once

#include "NewParticleDrawer.h"
#include "cinder/gl/gl.h"

using namespace std;

class AnimParticleDrawer : public NewParticleDrawer
{
public:
  AnimParticleDrawer (const uint32_t nofWidthImages,
                      const uint32_t nofHeightImages,
                      const uint32_t totalNofImages,
                      const string& vertexShaderName="pointspritesheet_es3.vert",
                      const string& fragmentShaderName="pointspritesheet_es3.frag");
  
  virtual ~AnimParticleDrawer ();
  
  // NewParticleDrawer operations
  void beforeDraw () override;
  
private:
  uint32_t m_nofWidthImages;
  uint32_t m_nofHeightImages;
  uint32_t m_totalNofImages;
};
