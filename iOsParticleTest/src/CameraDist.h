//
//  FireComet.hpp
//  iOsParticleTest
//
//  Created by Viktor Fägerlind on 02/10/16.
//
//

#pragma once

#include "cinder/gl/gl.h"

using namespace ci;
using namespace std;

class CameraEmitter;

class CameraDist
{
public:
  CameraDist ();

  SurfaceRef getImage ();
  
  CameraEmitter *m_cameraEmitter;
};

