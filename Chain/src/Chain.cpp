#include "Chain.h"
#include "cinder/gl/gl.h"

#include <math.h>

//----------------------------------------------------------------------------------------------------------------------

float ChainElement::g = 9.82f;

//----------------------------------------------------------------------------------------------------------------------

ChainElement::ChainElement (Vec3f position, float angle, float length, float mass)
{
  mPosition = position;
  mAngle    = angle;
  mLength   = length;
  mMass     = mass;

//  mAngleAcceleration  = 0.0f;
  mAngleSpeed         = 0.0f;

  mChild              = NULL;
}

//----------------------------------------------------------------------------------------------------------------------

ChainElement::~ChainElement ()
{
  if (mChild != NULL)
    delete mChild;
}

//----------------------------------------------------------------------------------------------------------------------

void ChainElement::addChild ()
{
  mChild = new ChainElement (getEndPosition (), mAngle, mLength, mMass);
}

//----------------------------------------------------------------------------------------------------------------------
float ChainElement::getLineForce ()
{
  return mAngleSpeed * mAngleSpeed * mLength * mMass;
}

//----------------------------------------------------------------------------------------------------------------------

Vec3f ChainElement::getEndPosition ()
{
  Vec3f endPosition (mPosition.x + mLength * cos (mAngle),
                     mPosition.y + mLength * sin (mAngle),
                     0.0f);

  return endPosition;
}

//----------------------------------------------------------------------------------------------------------------------

void ChainElement::update (const Vec3f& position)
{
  mPosition = position;

  if (mChild != NULL)
    mChild->update (getEndPosition ());

  const float dt = 1.0f / 60.0f;

  float tangentForce = mMass * ChainElement::g * cos (mAngle);

  float angleAcceleration = tangentForce / (mLength * mMass);

  // Euler...
  mAngleSpeed += angleAcceleration * dt;

  mAngle += mAngleSpeed;
}

//----------------------------------------------------------------------------------------------------------------------

void ChainElement::draw ()
{
  Vec3f endPosition = getEndPosition ();

  glColor4f (1.0f, 0, 0, 1.0f);

  gl::drawLine (mPosition, endPosition);

  gl::drawSolidCircle (Vec2f (endPosition.x, endPosition.y), mLength / 10.0f, 8);

  if (mChild != NULL)
    mChild->draw ();
}

//----------------------------------------------------------------------------------------------------------------------
