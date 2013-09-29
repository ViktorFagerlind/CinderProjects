#include "Parallax.h"

#include "cinder/app/App.h"
#include "cinder/Rand.h"

#include "ImageLibrary.h"
#include "Conversions.h"
#include "World.h"
#include "Emitter.h"

using namespace ci;
using namespace ci::app;

//----------------------------------------------------------------------------------------------------------------------

ParallaxLayer::ParallaxLayer (float distance)
: m_position (0.f, 0.f, distance)
{                                                                     
}

void ParallaxLayer::update (const float dt, const float speed)
{
  m_position.y -= speed;
}

void ParallaxLayer::draw (bool useBlending)
{
  float halfScreenHeight = World::getSingleton ().getTopLeft ().y;

  // Enable arrays
	glEnableClientState (GL_VERTEX_ARRAY);
	glEnableClientState (GL_TEXTURE_COORD_ARRAY);
	glEnableClientState (GL_COLOR_ARRAY);

  for (uint32_t i=0; i < m_objects.size (); i++)
  {
    ParallaxObject *o = &m_objects[i];

    if (useBlending == o->m_blend)
    {
      Vec3f position = m_position + Conversions::Vec2fTo3f (o->m_relativePosition);
//      if (position.y + o->m_size > -halfScreenHeight &&
//          position.y - o->m_size <  halfScreenHeight)
      {
        o->m_texture.bind ();
        Emitter::drawBillboard (position, Vec2f (o->m_size, o->m_size));
        o->m_texture.unbind ();
      }
    }
  }

  // Disable arrays
	glDisableClientState (GL_VERTEX_ARRAY);
	glDisableClientState (GL_TEXTURE_COORD_ARRAY);
	glDisableClientState (GL_COLOR_ARRAY);
}

void ParallaxLayer::drawSolid ()
{
  draw (false);
}

void ParallaxLayer::drawTransparent ()
{
  draw (true);
}

//----------------------------------------------------------------------------------------------------------------------

BottomLayer::BottomLayer ()
: m_bottomIndex      (0),
  m_topIndex         (1),
  m_bottomPosition   (0.f)
{                                                                     
  m_backgrounds.push_back (ImageLibrary::getSingleton ().getTexture ("space_background_2_01_03.jpg"));
  m_backgrounds.push_back (ImageLibrary::getSingleton ().getTexture ("space_background_2_01_02.jpg"));
  m_backgrounds.push_back (ImageLibrary::getSingleton ().getTexture ("space_background_2_01_01.jpg"));

  m_backgrounds.push_back (ImageLibrary::getSingleton ().getTexture ("space_background_2_02_03.jpg"));
  m_backgrounds.push_back (ImageLibrary::getSingleton ().getTexture ("space_background_2_02_02.jpg"));
  m_backgrounds.push_back (ImageLibrary::getSingleton ().getTexture ("space_background_2_02_01.jpg"));
}


void BottomLayer::update (const float dt, const float speed)
{
  float screenHeight = -2.f * World::getSingleton ().getDownRight ().y;

  m_bottomPosition -= speed * dt;

  if (m_bottomPosition < -screenHeight)
  {
    m_bottomIndex = m_topIndex;
    m_topIndex = (m_bottomIndex + 1) % m_backgrounds.size ();

    m_bottomPosition += screenHeight;
  }
}

void BottomLayer::drawSolid ()
{
  Vec2f halfScreenSize = Vec2f (1.f, -1.f) * World::getSingleton ().getDownRight ();

  // TODO remove!!!
	//gl::enableAlphaBlending ();
	//gl::enableAdditiveBlending ();




  // Enable arrays
	glEnableClientState (GL_VERTEX_ARRAY);
	glEnableClientState (GL_TEXTURE_COORD_ARRAY);
	glEnableClientState (GL_COLOR_ARRAY);

  // TODO remove!!!
//  for (uint32_t i=0; i<20; i++) 
  {
    m_backgrounds[m_bottomIndex].bind ();
    Emitter::drawBillboard (Vec3f (0.f, m_bottomPosition /*+ i*10.f*/, 0.f), 
                            Vec2f (halfScreenSize.x, halfScreenSize.y),
                            ColorAf (1,1,1,1));
    m_backgrounds[m_bottomIndex].unbind ();

    m_backgrounds[m_topIndex].bind ();
    Emitter::drawBillboard (Vec3f (0.f, m_bottomPosition + 2.f*halfScreenSize.y /*+ i*10.f*/, 0.f), 
                            Vec2f (halfScreenSize.x, halfScreenSize.y),
                            ColorAf (1,1,1,1));
    m_backgrounds[m_topIndex].unbind ();
  }

  // Disable arrays
	glDisableClientState (GL_VERTEX_ARRAY);
	glDisableClientState (GL_TEXTURE_COORD_ARRAY);
	glDisableClientState (GL_COLOR_ARRAY);
}


//----------------------------------------------------------------------------------------------------------------------

Parallax::Parallax ()
{                                                                     
  ParallaxObject o;

  {
    ParallaxLayer  l (-2000.f);

    o.m_blend    = false;
    o.m_size     = 400.f;
    o.m_texture  = ImageLibrary::getSingleton ().getTexture ("planet_02.png");

    for (uint32 i=0; i<100; i++)
    {
      o.m_relativePosition = Vec2f (Rand::randFloat (-2000.f, 2000.f), 2000.f * (float)i);
      l.addObject (o);
    }

    m_layers.push_back (l);
  }

  {
    ParallaxLayer  l (-100.f);

    o.m_blend    = false;
    o.m_size     = 400.f;
    o.m_texture  = ImageLibrary::getSingleton ().getTexture ("planet_01.png");

    for (uint32 i=0; i<100; i++)
    {
      o.m_relativePosition = Vec2f (Rand::randFloat (-800.f, 800.f), 2000.f * (float)i);
      l.addObject (o);
    }

    m_layers.push_back (l);
  }

  {
    ParallaxLayer  l (100.f);

    o.m_blend    = true;
    o.m_size     = 1000.f;
    o.m_texture  = ImageLibrary::getSingleton ().getTexture ("space_background_1.jpg");

    for (uint32 i=0; i<20; i++)
    {
      o.m_relativePosition = Vec2f (Rand::randFloat (-100.f, 100.f), 20000.f * (float)i);
      l.addObject (o);
    }

    m_layers.push_back (l);
  }
}

void Parallax::update (const float dt)
{
  const float speed = 20.f;

  m_bottomLayer.update (dt, speed);

  for (uint32_t i=0; i < m_layers.size (); i++)
    m_layers[i].update (dt, speed);
}

void Parallax::drawSolid ()
{
	gl::disableDepthWrite (); 
  m_bottomLayer.drawSolid ();
	gl::enableDepthWrite (); 

  gl::enableAlphaBlending ();
  glBlendFunc (GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

  for (uint32_t i=0; i < m_layers.size (); i++)
  {
    ParallaxLayer *l = &m_layers[i];

    l->drawSolid ();
  }

  gl::disableAlphaBlending ();
}

void Parallax::drawTransparent ()
{
  for (uint32_t i=0; i < m_layers.size (); i++)
  {
    ParallaxLayer *l = &m_layers[i];

    l->drawTransparent ();
  }
}

