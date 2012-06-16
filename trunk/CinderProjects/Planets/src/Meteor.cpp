#include "Meteor.h"

//----------------------------------------------------------------------------------------------------------------------
//---- MeteorDensity ---------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------

MeteorDensity::MeteorDensity ()
{
  mPerlin.setSeed (clock());

  mOctaves.push_back (Octave (1.0f/2.0f,    1.5f));
  mOctaves.push_back (Octave (1.0f/4.0f,    2.0f));
  mOctaves.push_back (Octave (1.0f/8.0f,    4.0f));
}

//----------------------------------------------------------------------------------------------------------------------

float MeteorDensity::f (const Vec3f& p)
{
  float baseFreq = 0.07f;

  float x = p.x * baseFreq;
  float y = p.y * baseFreq;
  float z = p.z * baseFreq;

  float noise = 0.0f;
  for (uint32_t i = 0; i < mOctaves.size (); i++)
    noise += mOctaves[i].weight * mPerlin.noise (x * mOctaves[i].freq, 
                                                  y * mOctaves[i].freq,
                                                  z * mOctaves[i].freq);

  return 4.0f * noise + p.length ()*0.5f - 4;
}

//----------------------------------------------------------------------------------------------------------------------
//---- Meteor ----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------

Meteor::Meteor (float mass, const Vec3f& position, float size, shared_ptr<BaseMaterial> material)
: DynamicObject (mass, position, size),
  mMesh (getMesh (Rand::randInt (20)))
{
  mHasCollided    = false;
  mCollideCounter = 0;

  mMaterial       = material;
}

//----------------------------------------------------------------------------------------------------------------------

void Meteor::update ()
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

//----------------------------------------------------------------------------------------------------------------------

void Meteor::draw() 
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

//----------------------------------------------------------------------------------------------------------------------

bool Meteor::isDead ()
{
  return mHasCollided         || 
          mCollideCounter > 50 ||
          mPosition.x < -500   || mPosition.x > 500 || 
          mPosition.y < -500   || mPosition.y > 500;
}

//----------------------------------------------------------------------------------------------------------------------

gl::VboMesh* Meteor::getMesh (uint32_t index)
{
  MeteorDensity densityFunction;
  static vector<gl::VboMesh*> meshes;

  if (index < meshes.size ())
    return meshes[index];

  gl::VboMesh *mesh = new gl::VboMesh ();
  IsoSurface::getSurfaceMesh (densityFunction, *mesh, Vec3f (20, 20, 20), Vec3i (20, 20, 20));
  meshes.push_back (mesh);

  return meshes.back ();
}
