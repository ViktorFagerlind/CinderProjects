#pragma once

#include "NewParticleDrawer.h"
#include "cinder/gl/gl.h"

using namespace std;

class AnimParticleDrawer : public NewParticleDrawer
{
public:
  AnimParticleDrawer (const string& vertexShaderName="pointspritesheet_es3.vert",
                      const string& fragmentShaderName="pointspritesheet_es3.frag");
  virtual ~AnimParticleDrawer ();
  
  // NewParticleDrawer operations
  void beforeDraw () override;
};
