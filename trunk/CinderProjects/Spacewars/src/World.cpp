#include "World.h"

#include "cinder/Timeline.h"
#include "cinder/gl/gl.h"
#include "cinder/Timeline.h"
#include "cinder/app/App.h"
#include "cinder/CinderMath.h"

#include "MovingCamera.h"
#include "Protagonist.h"
#include "Parallax.h"
#include "EnemyArrow.h"
#include "EnemyBot.h"
#include "ParticleSystemManager.h"

#include "Conversions.h"

using namespace ci;
using namespace ci::app;


class DebugDrawer : public b2Draw
{
public:
  void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
  {

  }

  void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
  {
    //set up vertex array
    float glverts[16]; //allow for polygons up to 8 vertices
    glVertexPointer(2, GL_FLOAT, 0, glverts); //tell OpenGL where to find vertices
    glEnableClientState(GL_VERTEX_ARRAY); //use vertices in subsequent calls to glDrawArrays
    
    //fill in vertex positions as directed by Box2D
    for (int i = 0; i < vertexCount; i++) 
    {
      glverts[i*2]   = Conversions::fromPhysics (vertices[i].x);
      glverts[i*2+1] = Conversions::fromPhysics (vertices[i].y);
    }
    
    //draw solid area
    gl::disable (GL_LIGHTING);
    glColor4f( color.r, color.g, color.b, 1);
    glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);
    gl::enable (GL_LIGHTING);
  
    //draw lines
    glLineWidth(3); //fat lines
    glColor4f( 1, 0, 1, 1 ); //purple
    glDrawArrays(GL_LINE_LOOP, 0, vertexCount);
  }

  void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
  {
  }

  void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
  {
    gl::drawSolidCircle (Conversions::fromPhysics (center), Conversions::fromPhysics (radius), 10);
  }

  void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
  {
  }

  void DrawTransform(const b2Transform& xf)
  {
  }

};

World::World ()
: m_gravity         (0.0f, 0.f),//-9.82f),
  m_physicsWorld    (m_gravity)
{
  m_physicsWorld.SetContactListener (&m_contactListener);

  m_camera.reset (new MovingCamera (1000.f, 20.f));

#ifdef _DEBUG
  m_debugDrawer.reset (new DebugDrawer);
  m_debugDrawer->SetFlags (b2Draw::e_shapeBit);
  m_physicsWorld.SetDebugDraw (m_debugDrawer.get ());
#endif
}

World::~World ()
{
}

void World::setup ()
{
  // Setup the matrices to get pixelToWorld, getTopLeft and getDownRight working
  m_camera->setMatrices ();

  m_protagonist.reset (new Protagonist ());

  m_previousTime = m_currentTime = timeline ().getCurrentTime ()*1000.f;

  m_parallax.reset (new Parallax ());
}

void World::keyDown(KeyEvent event) 
{
  m_camera->keyDown (event);
}

Vec2f World::pixelToWorld (const Vec2f& mousePos)
{
  float left, top, right, bottom, nearz, farz;

  const CameraPersp& cam = m_camera->getCam ();

  float distanceWidthScaler = 2.f * sinf (toRadians (cam.getFov ()/2.f));

  // ??? Varför behövs detta (speciellt *2) ???
  cam.getFrustum (&left, &top, &right, &bottom, &nearz, &farz);
  distanceWidthScaler *= 2.f * -left;

  const float worldWidth  = cam.getEyePoint().z*distanceWidthScaler;
  const float worldHeight = worldWidth / getWindowAspectRatio ();

  return Vec2f ((float)mousePos.x  / (float)getWindowWidth ()  * worldWidth - worldWidth/2.f,
                (float)-mousePos.y / (float)getWindowHeight () * worldHeight + worldHeight/2.f);
/*
  float u = ((float) mousePos.x) / getWindowWidth ();
  float v = ((float) (getWindowHeight () - mousePos.y)) / getWindowHeight ();
  Ray ray = m_camera->getCam ().generateRay (u, v, getWindowAspectRatio ());
    return Conversions::Vec3fTo2f (ray.calcPosition (1200.f));
*/
}

Vec2f World::getTopLeft () 
{
  return pixelToWorld (Vec2f (0, 0));
}

Vec2f World::getDownRight () 
{
  return pixelToWorld (Vec2f ((float)getWindowWidth (), (float)getWindowHeight ()));
}


b2World& World::getPhysicsWorld ()
{
  return getSingleton ().m_physicsWorld;
}

void World::issueNewObjects ()
{
  static float timeEnemyArrow = 0.f;
  static float timeEnemyBot   = 0.f;

  m_currentTime = timeline ().getCurrentTime ();


  if ((m_currentTime - timeEnemyArrow) > 1.5f)
  {
    m_objects.push_back (shared_ptr<WorldObject> (new EnemyArrow));
    timeEnemyArrow = m_currentTime;
  }

  if ( 5.f < m_currentTime && m_currentTime <  7.f ||
      15.f < m_currentTime && m_currentTime < 17.f ||
      25.f < m_currentTime && m_currentTime < 27.f ||
      35.f < m_currentTime && m_currentTime < 37.f ||
      45.f < m_currentTime && m_currentTime < 47.f ||
      55.f < m_currentTime && m_currentTime < 57.f ||
      65.f < m_currentTime && m_currentTime < 67.f ||
      75.f < m_currentTime && m_currentTime < 77.f)
  {
    if ((m_currentTime - timeEnemyBot) > 0.03f)
    {
      m_objects.push_back (shared_ptr<WorldObject> (new EnemyBot));
      timeEnemyBot = m_currentTime;
    }
  }

  m_previousTime = m_currentTime;
}

void World::update (const float dt, const Vec2f& touchPos)
{
	// --- update background ---------
  m_parallax->update (dt);

	// --- issue new object ----------
  issueNewObjects ();

	// --- update ship ---------------
  m_protagonist->update (dt, touchPos);

	// --- update objects ------------
  for (list<shared_ptr<WorldObject>>::iterator it=m_objects.begin (); it != m_objects.end ();)
  {
    shared_ptr<WorldObject> object = *it;

    if (!object->isDead ())
    {
      object->update (dt);
      it++;
    }
    else
      it = m_objects.erase (it);
  }

	// --- step physics world ------------
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	m_physicsWorld.Step (dt, velocityIterations, positionIterations);

	// --- update particle systems -------
  ParticleSystemManager::getSingleton().update ();
}

void World::draw ()
{
  // Setup camera
  m_camera->setMatrices ();

	// --- clear screen ------------------------
	gl::clear (Color (0, 0, 0));

	gl::disable (GL_LIGHTING);
  gl::disableAlphaBlending ();
	gl::disableDepthWrite ();
  m_parallax->drawBackground ();
  
  // --- draw solid objects ------------------
	gl::enable (GL_LIGHTING);
	gl::enableDepthRead (); 
	gl::enableDepthWrite (); 

	gl::disableAlphaBlending ();

	// draw ship 
  m_protagonist->drawSolid ();

	// draw world objects
  for (list<shared_ptr<WorldObject>>::iterator it=m_objects.begin (); it != m_objects.end (); it++)
  {
    shared_ptr<WorldObject> object = *it;
    object->drawSolid ();
  }

	// draw parallax (late to be able to cover objects, but still not
  // let transparent parts mess up the depth buffer
	gl::disable (GL_LIGHTING);
  m_parallax->drawSolid ();

	// draw debug physics world
  m_physicsWorld.DrawDebugData ();

	// --- draw transparent objects ------------------
	gl::disable (GL_LIGHTING);
	gl::disableDepthWrite ();
	gl::enableAlphaBlending ();
	gl::enableAdditiveBlending ();
	//glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE);

	// draw background
	m_parallax->drawTransparent ();

  // draw ship 
  m_protagonist->drawTransparent ();

	// draw world objects
  for (list<shared_ptr<WorldObject>>::iterator it=m_objects.begin (); it != m_objects.end (); it++)
  {
    shared_ptr<WorldObject> object = *it;
    object->drawTransparent ();
  }

  // draw particle systems
  ParticleSystemManager::getSingleton().draw ();

	// draw parallax
  m_parallax->drawTransparent ();
}

World& World::getSingleton ()
{
  static World instance;

  return instance;
}
