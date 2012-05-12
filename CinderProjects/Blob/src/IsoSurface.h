#pragma once

#include "cinder/Vector.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Vbo.h"
#include "../../Planets/src/PhongMaterial.h"

using namespace ci;
using namespace std;

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

  float getWidth ()  {return mGridWidth;}
  float getHeight () {return mGridHeight;}
  float getDepth ()  {return mGridDepth;}

private:
  void IsoSurface::getTetraCubeIndices (const uint32_t x, const uint32_t y, const uint32_t z, 
                                        std::vector<uint32_t>& vboIndices);

  void getIntersection (const Vec3f* verts, const float* evals, const uint8_t* indices, 
                        Vec3f* outVerts, uint32_t& nofVerts);

  void drawAllCubes ();

  void drawTetraCube (const Vec3f& pos, const float s);

  void drawTetrahedron (const Vec3f* verts, const float* evals, const uint8_t* indices);

private:
  void setupTetraVbo ();

  gl::VboMesh mVboMesh;

  const uint32_t mGridNofX;
  const uint32_t mGridNofY;
  const uint32_t mGridNofZ;

  const float mGridWidth;
  const float mGridHeight;
  const float mGridDepth;
};