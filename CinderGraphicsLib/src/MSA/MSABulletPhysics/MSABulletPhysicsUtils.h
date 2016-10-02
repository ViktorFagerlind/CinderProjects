/*
 *  BulletPhysicsUtils.h
 *  BulletPhysics demo
 *
 *  Created by Mehmet Akten on 21/06/2009.
 *  Copyright 2009 MSA Visuals Ltd.. All rights reserved.
 *
 */

#pragma once

#include "MSACore.h"
#include "btBulletDynamicsCommon.h"

namespace MSA {
	
	inline btVector3 vec3_To_btVector3(vec3 &p) {
		return btVector3(p.x, p.y, p.z);
	}
	
	inline const btVector3 vec3_To_btVector3(const vec3 &p) {
		return btVector3(p.x, p.y, p.z);
	}
	
	
	inline vec3 btVector3_To_vec3(btVector3 &p) {
		return vec3(p.m_floats[0], p.m_floats[1], p.m_floats[2]);
	}
	
	inline const vec3 btVector3_To_vec3(const btVector3 &p) {
		return vec3(p.m_floats[0], p.m_floats[1], p.m_floats[2]);
	}
	
}