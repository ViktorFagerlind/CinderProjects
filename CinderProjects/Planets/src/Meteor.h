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

  MeteorDensity ();

  float f (const Vec3f& p);

private:
  Perlin mPerlin;

  vector<Octave> mOctaves;
};


class Meteor : public DynamicObject, public BasicObject
{
public:
  Meteor (float mass, const Vec3f& position, float size, shared_ptr<BaseMaterial> material);

  void update ();

  void draw ();

  bool isDead ();

  static gl::VboMesh* getMesh (uint32_t index);

protected:
  bool          mHasCollided;
  uint32_t      mCollideCounter;

  gl::VboMesh  *mMesh;

  shared_ptr<BaseMaterial> mMaterial;
};

