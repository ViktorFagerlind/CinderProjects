#pragma once
#include "BoxParticle.h"
#include <Box2D/Box2d.h>
#include <list>

class BoxParticleController {
	public:
		BoxParticleController();
		// pass the physics world from the main app by reference
		void setup(b2World &w);
		void update();
		void draw();
		void addParticle(const ci::ivec2 &mousePos);
		void removeAll();
	
	private:
		// store a pointer to the physics world fro the main app
		b2World *world;
		std::list<BoxParticle>	particles;
};
