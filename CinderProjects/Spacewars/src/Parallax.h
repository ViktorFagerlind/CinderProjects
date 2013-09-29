#pragma once

#include "cinder/Cinder.h"
#include "cinder/Vector.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"

#include <vector>

using namespace ci;
using namespace std;

//----------------------------------------------------------------------------------------------------------------------

class ParallaxObject
{
public:
  gl::Texture m_texture;
  float       m_size;
  Vec2f       m_relativePosition;
  bool        m_blend;
};

//----------------------------------------------------------------------------------------------------------------------

class ParallaxLayer
{
public:
  ParallaxLayer (float distance);

  void addObject (const ParallaxObject &object) {m_objects.push_back (object);}

  void update (const float dt, const float speed);

  void drawSolid ();

  void drawTransparent ();

private:
  void draw (bool useBlending);

private:
  vector<ParallaxObject>  m_objects;

  Vec3f                   m_position;
};

//----------------------------------------------------------------------------------------------------------------------

class BottomLayer
{
public:
  BottomLayer ();

  void update (const float dt, const float speed);

  void drawSolid ();

private:
  vector<gl::Texture> m_backgrounds;

  uint32_t            m_bottomIndex;
  uint32_t            m_topIndex;

  float               m_bottomPosition;
};

//----------------------------------------------------------------------------------------------------------------------

class Parallax
{
public:
  Parallax ();

  void update (const float dt);

  void drawSolid ();

  void drawTransparent ();

private:
  BottomLayer           m_bottomLayer;

  vector<ParallaxLayer> m_layers;
};

//----------------------------------------------------------------------------------------------------------------------

