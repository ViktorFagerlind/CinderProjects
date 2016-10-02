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

typedef std::vector<vec3>   Field1D;
typedef std::vector<Field1D> Field2D;
typedef std::vector<Field2D> Field3D;

class GravityField : public Modifier
{
public:
  GravityField (const vec3& size, const ivec3& nofElements);

  void addGravityObject (PhysicsObject *object);
  void removeGravityObject (PhysicsObject *object);

  void addDynamicObject (DynamicObject *object);
  void removeDynamicObject (DynamicObject *object);

  void applyGravity ();

  bool isCollision (PhysicsObject *object);
  bool isStaticCollision (PhysicsObject *object);
  bool isDynamicCollision (PhysicsObject *object);

  // void resetField ();

  static vec3 calculateGravityForce (const vec3& subjectPosition, 
                                      float subjectMass, 
                                      const vec3& objectPosition, 
                                      float objectMass);

//  void update ();

  void draw ();

  // Modifier 
  inline void apply (Particle *const particle)
  {
    //particle->applyForce (3.0f * getGravityPotentialForObject (particle->getPosition ()));
  }    

private:
  const vec3 getGravityPotentialForObject (const vec3& objectPositon);

  void initMesh();

  uint32_t getIndexForMeshPosition (uint32_t x, uint32_t y, uint32_t z);

private:
  set <DynamicObject *> mDynamicObjects;

  list <PhysicsObject *> mGravityObjects;

  // Field3D mGravityPotential;

  const vec3 mPosition;
  const vec3 mSize;
  const vec3 mElementSize;
  const ivec3 mNofElements;

	// Shader
	gl::GlslProg          mShader;

  // Vbo
	gl::VboMesh	          mVboMesh;

  vector<float> mMasses;
  vector<float> mRadius;
  vector<vec3> mPositions;
};


