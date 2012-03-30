#pragma once

#include "cinder/gl/gl.h"

#include "GameWorld.h"
#include "DynamicObject.h"
#include "BasicObject.h"
#include "GravityField.h"

#include "../../ParticleTest/src/PointEmitter.h"

using namespace ci;
using namespace std;

class Meteor : public DynamicObject, public BasicObject
{
public:
  Meteor (float mass, const Vec3f& position, float size)
    :DynamicObject(mass, position, size)
  {
    mHasCollided    = false;
    mCollideCounter = 0;
  }

  void update ()
  {
    move ();

    if (GameWorld::getGravityField ()->isStaticCollision (this))
    {
      mHasCollided = true;
      ParticleSystem *particleSystem = GameWorld::getExplosionSystem ();

      PointEmitter *emitter = new PointEmitter (1000,                 //maxNofParticles,
                                                mPosition,            //position, 
  						                                  100,                   //particlesPerFrame, 
							                                  3,                    //minParticleSize,
							                                  7,                   //maxParticleSize,
							                                  mVelocity,            //baseVelocity,
							                                  Vec3f (0.6f, 0.6f, 0.6f));  //randVelocity
      emitter->update ();
      emitter->kill ();
      particleSystem->addEmitter (emitter);
    }
  }

  void draw() 
  {
    gl::pushMatrices ();

    gl::translate (mPosition);
    gl::rotate (mRotation);

    glColor3f (0.7f, 0.2f, 0.5f);
    gl::drawCube (Vec3f (0, 0, 0), Vec3f (mRadius, mRadius, mRadius));

    gl::popMatrices ();
  }

  bool isDead ()
  {
    return mHasCollided         || 
           mCollideCounter > 50 ||
           mPosition.x < -500   || mPosition.x > 500 || 
           mPosition.y < -500   || mPosition.y > 500;
  }

protected:
  bool          mHasCollided;
  unsigned int  mCollideCounter;
};


