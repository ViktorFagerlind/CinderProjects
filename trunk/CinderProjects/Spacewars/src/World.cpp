#include "World.h"

#include "cinder/Timeline.h"
#include "cinder/gl/gl.h"
#include "cinder/Timeline.h"
#include "cinder/app/App.h"
#include "cinder/CinderMath.h"
#include "Protagonist.h"

#include "Enemy1.h"
#include "EnemyBot.h"

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

#ifdef _DEBUG
  m_debugDrawer.reset (new DebugDrawer);
  m_debugDrawer->SetFlags (b2Draw::e_shapeBit);
  m_physicsWorld.SetDebugDraw (m_debugDrawer.get ());
#endif
}

void World::setup ()
{
  m_protagonist.reset (new Protagonist ());

  m_previousTime = m_currentTime = timeline ().getCurrentTime ()*1000.f;
}

World::~World ()
{
}

b2World& World::getPhysicsWorld ()
{
  return getSingleton ().m_physicsWorld;
}

void World::issueNewObjects ()
{
  static float timeEnemy1     = 0.f;
  static float timeEnemyBot   = 0.f;

  m_currentTime = timeline ().getCurrentTime ();


  if ((m_currentTime - timeEnemy1) > 2.f)
  {
    m_objects.push_back (shared_ptr<WorldObject> (new Enemy1));
    timeEnemy1 = m_currentTime;
  }

  if ((m_currentTime - timeEnemyBot) > 0.12f)
  {
    m_objects.push_back (shared_ptr<WorldObject> (new EnemyBot));
    timeEnemyBot = m_currentTime;
  }

  m_previousTime = m_currentTime;
}

void World::update (const float dt, const Vec2f& touchPos)
{
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
}

void World::draw ()
{
	// --- draw ship ---------------------
  m_protagonist->draw ();

	// --- draw objects ------------------
  for (list<shared_ptr<WorldObject>>::iterator it=m_objects.begin (); it != m_objects.end (); it++)
  {
    shared_ptr<WorldObject> object = *it;
    object->draw ();
  }

	// --- draw debug physics world ------
  m_physicsWorld.DrawDebugData ();
}

World& World::getSingleton ()
{
  static World instance;

  return instance;
}
