#pragma once

#include "cinder/Vector.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Vbo.h"
#include "PhongMaterial.h"

using namespace ci;
using namespace std;

class DensityInterface
{
public:
  virtual float f (const vec3& p) = 0;

  vec3 n (const vec3& p, float gridSize)
  {
    float d = gridSize * 2.0f;

    vec3 grad;  
    grad.x = f (p + vec3 (d, 0, 0)) - f (p + vec3 (-d, 0, 0));  
    grad.y = f (p + vec3 (0, d, 0)) - f (p + vec3 (0,-d, 0));  
    grad.z = f (p + vec3 (0, 0, d)) - f (p + vec3 (0, 0,-d));  

    return grad.normalized ();
  }
};


class IsoSurface
{
public:
  IsoSurface (const uint32_t  gridNofX    = 50,
              const uint32_t  gridNofY    = 50,
              const uint32_t  gridNofZ    = 50,
              const float     gridWidth   = 500.0f,
              const float     gridHeight  = 500.0f,
              const float     gridDepth   = 500.0f);

  void draw ();

  static void getSurfaceMesh (DensityInterface& densityFunction, gl::VboMesh& vboMesh, const vec3& GridSize, const ivec3& GridResolution);

  float getWidth ()  {return mGridWidth;}
  float getHeight () {return mGridHeight;}
  float getDepth ()  {return mGridDepth;}

private:
  static void IsoSurface::getTetraCubeIndices (const uint32_t x, const uint32_t y, const uint32_t z, const uint32_t gridNofX, const uint32_t gridNofY,
                                               std::vector<uint32_t>& vboIndices);

  static vec3 VertexInterp (float isolevel, vec3 p1, vec3 p2, float valp1, float valp2);

  static vec3 VertexInterp (DensityInterface& densityFunction, 
                             float isolevel, vec3 p1, vec3 p2, float valp1, float valp2);

  static void getIntersection (DensityInterface& densityFunction, 
                               const vec3* verts, const float* evals, const uint32_t* indices, 
                               vec3* outVerts, uint32_t& nofVerts);

  void drawAllCubes ();

  void drawTetraCube (const vec3& pos, const float s);

  void drawTetrahedron (const vec3* verts, const float* evals, const uint32_t* indices);

  void setupTetraVbo ();

  static void setupTetraGrid (std::vector<uint32_t>& vboIndices, std::vector<vec3>& vboVertices, const vec3& GridSize, const ivec3& GridResolution);

private:
  gl::VboMesh mVboMesh;

  const uint32_t mGridNofX;
  const uint32_t mGridNofY;
  const uint32_t mGridNofZ;

  const float mGridWidth;
  const float mGridHeight;
  const float mGridDepth;

  // Borde inte vara del av klassen...
	std::vector<uint32_t> mVboIndices;
	std::vector<vec3>    mVboVertices;
};