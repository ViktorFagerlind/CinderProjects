//
//  ArmJoint.cpp
//  HairyBall
//
//  Created by Viktor Fägerlind on 25/08/16.
//
//

#include "Ball.h"

#include "cinder/Rand.h"

#include "ContactListener.h"

using namespace std;
using namespace ci;


Physics::Physics ()
{
  b2Vec2 gravity (0, -9.82);
  
  m_world.reset (new b2World (gravity));
  
  m_world->Step (0.01, 1, 1 );
  
  m_world->SetContactListener (&m_contactListener);
  
  
  b2BodyDef bodyDef;
  bodyDef.type = b2_staticBody;
  bodyDef.allowSleep = false;
  bodyDef.position.Set (0, 0);
  
  b2CircleShape shape;
  shape.m_radius = Physics::cameraToMeters (1.f);
  
  b2FixtureDef fixtureDef;
  fixtureDef.shape                = &shape;
  fixtureDef.filter.categoryBits  = 0x0001;
  fixtureDef.filter.maskBits      = 0x0000;
  
  b2World *world = m_world.get ();
  
  m_centerPointBody.reset (m_world->CreateBody (&bodyDef), [world](b2Body *body) {world->DestroyBody (body);});
  m_centerPointBody->CreateFixture (&fixtureDef);
  
  
  
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set (0, 0);
  
  fixtureDef.density              = 1.0f;
  fixtureDef.friction             = 0.0f;
  fixtureDef.restitution          = 1.0f;
  fixtureDef.filter.categoryBits  = 0x0001;
  fixtureDef.filter.maskBits      = 0x0001;
  
  m_mainBody.reset (m_world->CreateBody (&bodyDef), [world](b2Body *body) {world->DestroyBody (body);});
  m_mainBody->CreateFixture (&fixtureDef);
  
  b2DistanceJointDef jointDef;
  jointDef.Initialize (m_centerPointBody.get(),
                       m_mainBody.get(),
                       m_centerPointBody->GetWorldCenter(),
                       m_mainBody->GetWorldCenter());
  jointDef.collideConnected = true;
  jointDef.frequencyHz      = 0.8f;
  jointDef.dampingRatio     = .2;
  
  m_joint.reset (world->CreateJoint (&jointDef), [world](b2Joint *joint) {world->DestroyJoint (joint);});
};


Physics::~Physics ()
{
  // make sure the joint is destroyed before the body...
  m_joint.reset();
}

void Physics::update (float deltaTime)
{
  m_world->Step(deltaTime, 8, 3 );
}


void Physics::draw ()
{
  gl::color (.1f,.2f,1.f);
  vec2 center = Physics::metersToCamera (m_centerPointBody->GetPosition());
  
  gl::drawSolidCircle (center, .1f);
}

ArmJoint::ArmJoint (shared_ptr<b2Body>	parent, const float radius, const float length, const bool collide)
{
  m_parent = parent;
  m_radius = radius;
  
  create (length, collide);
}

ArmJoint::~ArmJoint ()
{
  // make sure the joint is destroyed before the body...
  m_joint.reset();
}

void ArmJoint::create (const float length, const bool collide)
{
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.allowSleep = false;
  
  float angle = randFloat (0.f, 360.f);
  bodyDef.position.Set (m_parent->GetWorldCenter().x + cos(angle)*length, m_parent->GetWorldCenter().y + sin(angle)*length);
  
  b2CircleShape shape;
  shape.m_radius = Physics::cameraToMeters (m_radius);
  
  b2FixtureDef fixtureDef;
  fixtureDef.shape                = &shape;
  fixtureDef.density              = 1.0f;
  fixtureDef.friction             = 0.0f;
  fixtureDef.restitution          = 1.0f;
  fixtureDef.filter.categoryBits  = 0x0001;
  fixtureDef.filter.maskBits      = collide ? 0x0001 : 0x0000;
  
  b2World *world = Physics::getWorld ();
  m_body.reset (world->CreateBody (&bodyDef), [world](b2Body *body) {world->DestroyBody (body);});
  
  m_body->CreateFixture (&fixtureDef);
  m_body->SetUserData (this);
  
  /*
  b2DistanceJointDef jointDef;
  jointDef.Initialize (m_parent.get(),
                       m_body.get(),
                       m_parent->GetWorldCenter(),
                       m_body->GetWorldCenter());
  jointDef.collideConnected = true;
  jointDef.frequencyHz = 4.0f;
  jointDef.dampingRatio = 0.5f;
  */
  b2RopeJointDef jointDef;
  jointDef.maxLength = length;
  jointDef.bodyA = m_parent.get();
  jointDef.bodyB = m_body.get();
	jointDef.localAnchorA.Set(0.f, 0.f);
	jointDef.localAnchorB.Set(0.f, 0.f);
  jointDef.collideConnected = false;
  
  m_joint.reset (world->CreateJoint (&jointDef), [world](b2Joint *joint) {world->DestroyJoint (joint);});
  
}

Arm::Arm (const uint32_t nofJoints, gl::BatchRef endObjectBatch)
{
  m_endObjectBatch = endObjectBatch;
  
  create (nofJoints, Physics::getInstance().m_mainBody);

  m_bSplinePoints.resize(nofJoints+1);
}

void Arm::create (const uint32_t nofJoints, shared_ptr<b2Body> parent)
{
  shared_ptr<ArmJoint> joint;
  
  if (nofJoints == 0)
    return;
  
  joint.reset (new ArmJoint (parent, nofJoints == 1 ? .4 : .1f, 1.5f, nofJoints == 1));
  
  m_joints.push_back (joint);
  
  create (nofJoints-1, joint->m_body);
}

void Arm::update ()
{
  
}

void Arm::draw ()
{
//  gl::color (1.f,.2f,.1f);
//  gl::lineWidth (1);
//  gl::begin( GL_LINE_STRIP );
//    gl::vertex (Physics::metersToCamera ((*m_joints.begin())->m_parent->GetPosition()));
    m_bSplinePoints[0] = Physics::metersToCamera3d (m_joints[0]->m_parent->GetPosition());
    for (int i=0; i<m_joints.size(); i++)
    {
//      gl::vertex (Physics::metersToCamera (m_joints[i]->m_body->GetPosition()));
      m_bSplinePoints[i+1] = Physics::metersToCamera3d (m_joints[i]->m_body->GetPosition());
    }
  gl::end();
  
  BSpline3f bs (m_bSplinePoints, 3, false, true);

  /*
  Shape2d circle;
  circle.moveTo(vec2(-.1f, -.1f));
  circle.lineTo(vec2(-.1f,  .1f));
  circle.lineTo(vec2( .1f,  .1f));
  circle.lineTo(vec2( .1f, -.1f));
  circle.lineTo(vec2(-.1f, -.1f));
  
  gl::VboMeshRef  thickSpline  = gl::VboMesh::create( geom::ExtrudeSpline(circle, bs));
  gl::draw (thickSpline);
  */
  
  gl::color (.1f,.2f,1.f);
  gl::lineWidth (5);
  
  gl::VboMeshRef  lineSpline  = gl::VboMesh::create(geom::BSpline (bs, 20));
  gl::draw (lineSpline);
  
  
  /*
  gl::begin( GL_LINE_STRIP );
    BSpline3f bSpline (m_bSplinePoints, 3, false, true);
    uint32_t nofSegments = 10UL;
    float dt = 1.0f/(float)nofSegments;
    for (int i = 0; i < nofSegments+1; i++)
    {
      float t = i*dt;
      gl::vertex (bSpline.getPosition (t));
    }
  gl::end();
  */
  
  
  
  gl::color (1.f,.2f,.1f);

  gl::pushModelView();
  vector<shared_ptr<ArmJoint>>::iterator ajit = --m_joints.end();
  
  gl::translate(Physics::metersToCamera ((*ajit)->m_body->GetPosition()));
  gl::scale ((*ajit)->m_radius, (*ajit)->m_radius, (*ajit)->m_radius);
  m_endObjectBatch->draw ();
  
  gl::popModelView();
}



