#pragma once
#include "cinder/Color.h"
#include <Box2D/Box2d.h>

class BoxParticle
{
	public:
		BoxParticle();
		~BoxParticle();
		
		// pass in a pointer to the particle
		void setup(ci::vec2 boxSize);
		void update();
		void draw();
		
	//private:
		// store a pointer to the particle in the physics world from the main app
		b2Body* body;
		ci::Color color;
		ci::vec2 size;
};
