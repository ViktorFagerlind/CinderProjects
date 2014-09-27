#pragma once

#include "DynamicObject.h"

#include "Modifier.h"

#include "cinder/Vector.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Vbo.h"

#include <set>
#include <list>
#include <vector>

using namespace std;

typedef std::vector<Vec3f>   Field1D;
typedef std::vector<Field1D> Field2D;
typedef std::vector<Field2D> Field3D;

class GravityField : public Modifier
{
public:
  GravityField (const Vec3f& size, const Vec3i& nofElements);

  void addGravityObject (PhysicsObject *object);
  void removeGravityObject (PhysicsObject *object);

  void addDynamicObject (DynamicObject *object);
  void removeDynamicObject (DynamicObject *object);

  void applyGravity ();

  bool isCollision (PhysicsObject *object);
  bool isStaticCollision (PhysicsObject *object);
  bool isDynamicCollision (PhysicsObject *object);

  // void resetField ();

  static Vec3f calculateGravityForce (const Vec3f& subjectPosition, 
                                      float subjectMass, 
                                      const Vec3f& objectPosition, 
                                      float objectMass);

//  void update ();

  void draw ();

  // Modifier 
  inline void apply (Particle *const particle)
  {
    //particle->applyForce (3.0f * getGravityPotentialForObject (particle->getPosition ()));
  }    

private:
  const Vec3f getGravityPotentialForObject (const Vec3f& objectPositon);

  void initMesh();

  uint32_t getIndexForMeshPosition (uint32_t x, uint32_t y, uint32_t z);

private:
  set <DynamicObject *> mDynamicObjects;

  list <PhysicsObject *> mGravityObjects;

  // Field3D mGravityPotential;

  const Vec3f mPosition;
  const Vec3f mSize;
  const Vec3f mElementSize;
  const Vec3i mNofElements;

	// Shader
	gl::GlslProg          mShader;

  // Vbo
	gl::VboMesh	          mVboMesh;

  vector<float> mMasses;
  vector<float> mRadius;
  vector<Vec3f> mPositions;
};


