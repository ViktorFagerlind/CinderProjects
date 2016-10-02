#pragma once

//
//  ArmJoint.hpp
//  HairyBall
//
//  Created by Viktor Fägerlind on 25/08/16.
//
//

#include <Box2D/Box2D.h>

#include "cinder/gl/gl.h"
#include "ContactListener.h"

using namespace std;
using namespace ci;


const float POINTS_PER_METER = 1.f;

class Physics
{
public:
  Physics ();
  virtual ~Physics ();
  
  static b2World* getWorld ()
  {
    return getInstance ().m_world.get();
  }
  
  static Physics& getInstance()
  {
    static Physics instance;
    return instance;
  }
  
  void update (float deltaTime);
  
  void draw ();
  
  vec2 getMainBodyPosition () {return metersToCamera (m_mainBody->GetPosition());}
  
  static float   metersToCamera (const float    var) { return var * POINTS_PER_METER; }
  static float   cameraToMeters (const float    var) { return var / POINTS_PER_METER; }
  
  
  static vec2    metersToCamera (const b2Vec2 &var)  { return vec2 (var.x, var.y) * POINTS_PER_METER; }
  static vec3    metersToCamera3d (const b2Vec2 &var)  { return vec3 (var.x, var.y, 0.f) * POINTS_PER_METER; }
  static b2Vec2  cameraToMeters (const vec2   &var)  { return (1.f/POINTS_PER_METER) * b2Vec2 (var.x, var.y); }
  
  void applyForceToMainBall (const b2Vec2& force) {m_mainBody->ApplyForceToCenter (force);}
  
  
public:
  shared_ptr<b2World>	m_world;
  
  shared_ptr<b2Body>	m_centerPointBody;
  shared_ptr<b2Body>	m_mainBody;
  shared_ptr<b2Joint>	m_joint;
  
  ContactListener     m_contactListener;
};


class ArmJoint
{
  friend class Arm;
  
public:
  ArmJoint (shared_ptr<b2Body>	parent, const float radius, const float length, const bool collide);
  virtual ~ArmJoint ();
  
private:
  void create (const float length, const bool collide);
  
private:
  gl::TextureRef      m_imageTex;
  shared_ptr<b2Body>	m_parent;
  shared_ptr<b2Body>	m_body;
  shared_ptr<b2Joint>	m_joint;
  float               m_radius;
};

class Arm
{
public:
  Arm (const uint32_t nofJoints, gl::BatchRef endObjectBatch);
  
  void update ();
  
  void draw ();
  
private:
  void create (const uint32_t nofJoints, shared_ptr<b2Body> parent);
  
private:
  vector<shared_ptr<ArmJoint>>  m_joints;
  
  vector<vec3>                  m_bSplinePoints;
  
  gl::BatchRef                  m_endObjectBatch;
  
  //gl::Mesh                  m_endObjectBatch;
};




