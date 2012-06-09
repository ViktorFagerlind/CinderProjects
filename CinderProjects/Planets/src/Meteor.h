#pragma once

#include "cinder/gl/gl.h"

#include "GameWorld.h"
#include "DynamicObject.h"
#include "BasicObject.h"
#include "GravityField.h"
#include "cinder/Perlin.h"

#include "../../Blob/src/IsoSurface.h"
#include "../../ParticleTest/src/PointEmitter.h"

using namespace ci;
using namespace std;


class MeteorDensity : public DensityInterface
{
public:
  class Octave
  {
  public:
    Octave (float w, float f) 
    {
      weight = w; 
      freq = f;
    }

    float weight, freq;
  };

  MeteorDensity ()
  {
    mPerlin.setSeed (clock());

    mOctaves.push_back (Octave (1.0f/2.0f,    1.5f));
    mOctaves.push_back (Octave (1.0f/4.0f,    2.0f));
    mOctaves.push_back (Octave (1.0f/8.0f,    4.0f));
  }

  float f (const Vec3f& p)
  {
    float baseFreq = 1.0f/20.0f;

    float x = p.x * baseFreq;
    float y = p.y * baseFreq;
    float z = p.z * baseFreq;

    float noise = 0.0f;
    for (uint32_t i = 0; i < mOctaves.size (); i++)
      noise += mOctaves[i].weight * mPerlin.noise (x * mOctaves[i].freq, 
                                                   y * mOctaves[i].freq,
                                                   z * mOctaves[i].freq);

    return 13.0f * noise + p.length ()*0.8f - 10;
  }

private:
  Perlin mPerlin;

  vector<Octave> mOctaves;
};


class Meteor : public DynamicObject, public BasicObject
{
public:
  Meteor (float mass, const Vec3f& position, float size, shared_ptr<BaseMaterial> material)
  : DynamicObject (mass, position, size),
    mMesh (getMesh (Rand::randInt (20)))
  {
    mHasCollided    = false;
    mCollideCounter = 0;

    mMaterial       = material;
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
  						                                  100,                  //particlesPerFrame, 
							                                  3,                    //minParticleSize,
							                                  7,                    //maxParticleSize,
							                                  mVelocity,            //baseVelocity,
                                                0.1f,
							                                  0.6f);  //randVelocity
      emitter->updateEmitter ();
      emitter->kill ();
      particleSystem->addEmitter (emitter);
    }
  }

  void draw() 
  {
	  glDisable (GL_BLEND);
    glDisable (GL_TEXTURE_2D);

    gl::pushMatrices ();

    gl::translate (mPosition);
    gl::rotate (mRotation);

    //glColor3f (0.6f, 0.4f, 0.3f);
    //gl::drawCube (Vec3f (0, 0, 0), Vec3f (mRadius, mRadius, mRadius));

    mMaterial->bind ();
    gl::draw (*mMesh);
    mMaterial->unbind ();

    gl::popMatrices ();
  }

  bool isDead ()
  {
    return mHasCollided         || 
           mCollideCounter > 50 ||
           mPosition.x < -500   || mPosition.x > 500 || 
           mPosition.y < -500   || mPosition.y > 500;
  }

  static gl::VboMesh* getMesh (uint32_t index)
  {
    MeteorDensity densityFunction;
    static vector<gl::VboMesh*> meshes;

    if (index < meshes.size ())
      return meshes[index];

    gl::VboMesh *mesh = new gl::VboMesh ();
    IsoSurface::getSurfaceMesh (densityFunction, *mesh, Vec3f (30, 30, 30), Vec3i (20, 20, 20));
    meshes.push_back (mesh);

    return meshes.back ();
  }

protected:
  bool          mHasCollided;
  uint32_t      mCollideCounter;

  gl::VboMesh  *mMesh;

  shared_ptr<BaseMaterial> mMaterial;
};


