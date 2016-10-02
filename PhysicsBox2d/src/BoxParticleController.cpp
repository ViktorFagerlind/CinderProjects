#pragma once
#include "cinder/app/AppBasic.h"
#include "cinder/Rand.h"
#include "cinder/Vector.h"
#include "BoxParticleController.h"
#include "Globals.h"
#include "Conversions.h"

using namespace ci;
using std::list;

BoxParticleController::BoxParticleController()
{
}

void BoxParticleController::setup(b2World &w)
{
	world = &w;
}

void BoxParticleController::update()
{
	for( list<BoxParticle>::iterator p = particles.begin(); p != particles.end(); p++) {
		p->update();
	}
}

void BoxParticleController::draw()
{
	for( list<BoxParticle>::iterator p = particles.begin(); p != particles.end(); p++ ){
		p->draw();
	}
}

void BoxParticleController::addParticle(const ivec2 &mousePos)
{
	BoxParticle p = BoxParticle();

	// create a dynamic body
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set (Conversions::toPhysics(mousePos.x), Conversions::toPhysics(mousePos.y));

	// instead of just creating body... 
	// b2Body* body = world->CreateBody(&bodyDef);
	// do the following to create it with a circular reference to it's corresponding particle
	bodyDef.userData = &p;
	p.body = world->CreateBody(&bodyDef);

	b2PolygonShape dynamicBox;
	float boxSizeX = Rand::randFloat(global::BOX_X_MIN, global::BOX_X_MAX);
	float boxSizeY = Rand::randFloat(global::BOX_Y_MIN, global::BOX_Y_MAX);

	dynamicBox.SetAsBox(Conversions::toPhysics(boxSizeX), Conversions::toPhysics(boxSizeY));

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	fixtureDef.restitution = 0.5f; // bounce

	p.body->CreateFixture(&fixtureDef);

	// rest of initialization particle can do for itself
	p.setup(vec2(boxSizeX, boxSizeY));
	particles.push_back(p);

}

void BoxParticleController::removeAll()
{
	// not the right way, but an alternative
	/*
	b2Body* body = world->GetBodyList();
	while( body != NULL )
	{

		body = body->GetNext();
	}
	*/

	// right way
	for( list<BoxParticle>::iterator p = particles.begin(); p != particles.end(); p++) {
		world->DestroyBody(p->body);
	}

	particles.clear();

	if (global::COLOR_SCHEME == 0)
		global::COLOR_SCHEME++;
	else
		global::COLOR_SCHEME--;
}
