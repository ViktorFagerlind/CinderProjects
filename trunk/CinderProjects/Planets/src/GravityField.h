#pragma once

#include "DynamicObject.h"

#include "../../ParticleTest/src/Modifier.h"

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

typedef const Vec3<uint32_t> Vec3i;


class GravityField : public Modifier
{
public:
  GravityField (const Vec3f& position, const Vec3f& size, const Vec3i& nofElements);

  void addGravityObject (PhysicsObject *object);
  void removeGravityObject (PhysicsObject *object);

  void addDynamicObject (DynamicObject *object);
  void removeDynamicObject (DynamicObject *object);

  void applyGravity ();

  bool isCollision (PhysicsObject *object);
  bool isStaticCollision (PhysicsObject *object);
  bool isDynamicCollision (PhysicsObject *object);

  void resetField ();

  static Vec3f calculateGravityForce (const Vec3f& subjectPosition, 
                                      float subjectMass, 
                                      const Vec3f& objectPosition, 
                                      float objectMass);

  void update ();
  void draw ();

  // Modifier 
  inline void apply (Particle *const particle)
  {
    particle->applyForce (3.0f * getGravityPotentialForObject (particle->getPosition ()));
  }    

private:
  const Vec3f getGravityPotentialForObject (const Vec3f& objectPositon);

private:
  set <DynamicObject *> mDynamicObjects;

  list <PhysicsObject *> mGravityObjects;

  Field3D mGravityPotential;

  const Vec3f mPosition;
  const Vec3f mSize;
  const Vec3f mElementSize;
  const Vec3i mNofElements;

  float mElementsSize;
  int   mNofElementsPerSide;

	// Shader
	gl::GlslProg mShader;

	std::vector<uint32_t> mVboIndices;
	gl::VboMesh::Layout   mVboLayout;
	std::vector<Vec2f>    mVboTexCoords;
	std::vector<Vec3f>    mVboVertices;
	gl::VboMesh	          mVboMesh;

};


