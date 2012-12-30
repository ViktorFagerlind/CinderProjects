/*
 *  BulletRigidWorld.h
 *  BulletPhysics demo
 *
 *  Created by Mehmet Akten on 21/06/2009.
 *  Copyright 2009 MSA Visuals Ltd.. All rights reserved.
 *
 */

// container for rigid body objects only

#pragma once

#include "MSABulletWorldBase.h"

#include "cinder/gl/gl.h"

namespace MSA {
	
class CinderDebugDrawer : public btIDebugDraw
{
public:
  CinderDebugDrawer ()
  {
    mode = DBG_MAX_DEBUG_DRAW_MODE;
  }

  void drawLine(const btVector3& from,const btVector3& to,const btVector3& color)
  {
    gl::color (color.getX(), color.getY(), color.getZ());
    gl::drawLine (btVector3_To_Vec3f (from), btVector3_To_Vec3f (to));
  }

	void drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color)
  {
  }

	void reportErrorWarning(const char* warningString)
  {
  }

	void draw3dText(const btVector3& location,const char* textString)
  {
  }
	
	void setDebugMode(int debugMode)
  {
    mode = debugMode;
  }
	
	int getDebugMode() const
  {
    return mode;
  }

private:
  int mode;
};


class BulletRigidWorld : public BulletWorldBase 
{
public:
		
		
protected:
  btDynamicsWorld* createBulletWorld() 
  {
		btDynamicsWorld* world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
        
    world->setDebugDrawer (&debugDrawer);

    return world;
	}

private: 
  CinderDebugDrawer debugDrawer;
};


}