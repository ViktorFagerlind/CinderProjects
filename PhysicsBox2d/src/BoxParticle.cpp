#include "BoxParticle.h"
#include "Globals.h"
#include "cinder/Rand.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "Conversions.h"
#include <Box2D/Box2d.h>

BoxParticle::BoxParticle() 
{
}
	
BoxParticle::~BoxParticle() 
{
	//body->GetWorld()->DestroyBody( body );  // this ruins everything
}
	
	
void BoxParticle::setup(vec2 boxSize) 
{

	size = boxSize;

	if (global::COLOR_SCHEME == 0)
		color = ci::ColorA(1, ci::Rand::randFloat(0,.8), 0, 1);  // red to yellow
	else
		color = ci::ColorA(ci::Rand::randFloat(0,.8), 0, 1, 1);  // blue to violet
}
	
void BoxParticle::update() 
{
}

void BoxParticle::draw() 
{
  gl::color (color);

	vec2 pos = Conversions::toScreen( body->GetPosition() );
	float t = Conversions::radiansToDegrees( body->GetAngle() );
	
	glPushMatrix();
	gl::translate( pos );
	gl::rotate( t );
			   
	Rectf rect(-size.x, -size.y, size.x, size.y);
	gl::drawSolidRect(rect);
	
	glPopMatrix();
}

