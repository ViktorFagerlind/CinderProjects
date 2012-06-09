#pragma once

#include "cinder/Vector.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Vbo.h"
#include "../../Planets/src/PhongMaterial.h"

using namespace ci;
using namespace std;

class DensityInterface
{
public:
  virtual float f (const Vec3f& p) = 0;

  Vec3f n (const Vec3f& p, float gridSize)
  {
    float d = gridSize * 2.0f;

    Vec3f grad;  
    grad.x = f (p + Vec3f (d, 0, 0)) - f (p + Vec3f (-d, 0, 0));  
    grad.y = f (p + Vec3f (0, d, 0)) - f (p + Vec3f (0,-d, 0));  
    grad.z = f (p + Vec3f (0, 0, d)) - f (p + Vec3f (0, 0,-d));  

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

  static void getSurfaceMesh (DensityInterface& densityFunction, gl::VboMesh& vboMesh, const Vec3f& GridSize, const Vec3i& GridResolution);

  float getWidth ()  {return mGridWidth;}
  float getHeight () {return mGridHeight;}
  float getDepth ()  {return mGridDepth;}

private:
  static void IsoSurface::getTetraCubeIndices (const uint32_t x, const uint32_t y, const uint32_t z, const uint32_t gridNofX, const uint32_t gridNofY,
                                               std::vector<uint32_t>& vboIndices);

  static Vec3f VertexInterp (float isolevel, Vec3f p1, Vec3f p2, float valp1, float valp2);

  static Vec3f VertexInterp (DensityInterface& densityFunction, 
                             float isolevel, Vec3f p1, Vec3f p2, float valp1, float valp2);

  static void getIntersection (DensityInterface& densityFunction, 
                               const Vec3f* verts, const float* evals, const uint32_t* indices, 
                               Vec3f* outVerts, uint32_t& nofVerts);

  void drawAllCubes ();

  void drawTetraCube (const Vec3f& pos, const float s);

  void drawTetrahedron (const Vec3f* verts, const float* evals, const uint32_t* indices);

  void setupTetraVbo ();

  static void setupTetraGrid (std::vector<uint32_t>& vboIndices, std::vector<Vec3f>& vboVertices, const Vec3f& GridSize, const Vec3i& GridResolution);

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
	std::vector<Vec3f>    mVboVertices;
};