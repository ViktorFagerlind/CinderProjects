//
//  Flame.hpp
//  iOsParticleTest
//
//  Created by Viktor Fägerlind on 02/10/16.
//
//

#pragma once

#include "cinder/gl/gl.h"

using namespace cinder;
using namespace std;

class Emitter;
class ParticleSystem;

class Flame
{
public:
  Flame ();
  virtual ~Flame ();
  
  void setPosition (vec3 pos);
  
private:
  vector<shared_ptr<Emitter>>  m_emitters;
  vector<ParticleSystem*>     m_particleSystems;
};

