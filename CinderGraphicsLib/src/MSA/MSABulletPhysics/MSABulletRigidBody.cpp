/*
 *  BulletRigidBody.cpp
 *  BulletPhysics demo
 *
 *  Created by Mehmet Akten on 21/06/2009.
 *  Copyright 2009 MSA Visuals Ltd.. All rights reserved.
 *
 */

#include "MSABulletRigidBody.h"
#include "MSABulletPhysicsUtils.h"

namespace MSA {
	
	BulletRigidBody::BulletRigidBody() {
		m_bulletBody			= NULL;
		m_collisionShape	= NULL;
	}
	
	BulletRigidBody::~BulletRigidBody() 
  {
		DelPointer(m_bulletBody);					// object owns bulletBody, so delete when done

		if(m_ownsCollisionShape) 
      DelPointer(m_collisionShape);
	}
	
	void BulletRigidBody::setup (vec3 &startPosition, btCollisionShape *collisionShape, bool ownsCollisionShape, float mass, float restitution)
  {
		m_ownsCollisionShape = ownsCollisionShape;
		m_collisionShape = collisionShape;
		
		btTransform startTransform;
		startTransform.setIdentity();
		bool isDynamic = (mass != 0.f);
		
		btVector3 localInertia(0,0,0);
		if (isDynamic) m_collisionShape->calculateLocalInertia(mass,localInertia);
		
		startTransform.setOrigin(vec3_To_btVector3(startPosition));	
		
		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, collisionShape, localInertia);
		m_bulletBody = new btRigidBody(rbInfo);
		//body->setContactProcessingThreshold(collisionShape->getContactBreakingThreshold());
		//bulletBody->setActivationState(ISLAND_SLEEPING);

    m_bulletBody->setRestitution (restitution);
	}
}
