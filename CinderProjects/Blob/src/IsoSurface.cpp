#include "IsoSurface.h"

#include "Macros.h"
#include "cinder/CinderMath.h"
#include "cinder/gl/Vbo.h"

const float isoLimit = 0.0f;


IsoSurface::IsoSurface (const uint32_t  gridNofX,
                        const uint32_t  gridNofY,
                        const uint32_t  gridNofZ,
                        const float     gridWidth,
                        const float     gridHeight,
                        const float     gridDepth)
: mGridNofX   (gridNofX),
  mGridNofY   (gridNofY),
  mGridNofZ   (gridNofZ),
  mGridWidth  (gridWidth),
  mGridHeight (gridHeight),
  mGridDepth  (gridDepth)
{
  setupTetraVbo ();
}


void IsoSurface::getSurfaceMesh (DensityInterface& densityFunction, gl::VboMesh& vboMesh)
{
  float *evals = new float[mVboVertices.size ()];

  std::vector<uint32_t> indices;
  std::vector<Vec3f>    vertices;
  std::vector<Vec3f>    normals;

  Vec3f     intersectVerts[4];
  uint32_t  nofIntersectVerts;

  indices.clear ();
  vertices.clear ();

  // gl::VboMesh::VertexIter iter = mVboMesh.mapVertexBuffer(); Varför krashar detta !?!?!?!?!?!?

  for( uint32_t i = 0; i < mVboVertices.size (); i++)
  {
    evals[i] = densityFunction.f (mVboVertices[i]);
  }

  for( uint32_t i = 0; i < mVboIndices.size (); i+=4)
  {
    getIntersection (densityFunction, mVboVertices.data(), evals, &mVboIndices.data()[i], 
                     intersectVerts, nofIntersectVerts);

    uint32_t startIndex = vertices.size ();

    float d = min (min ((float)mGridNofX/mGridWidth, (float)mGridNofY/mGridHeight), (float)mGridNofZ/mGridDepth);

    if (nofIntersectVerts >= 3)
    {
      vertices.push_back (intersectVerts[0]);
      vertices.push_back (intersectVerts[1]);
      vertices.push_back (intersectVerts[2]);

      normals.push_back (densityFunction.n (intersectVerts[0], d));
      normals.push_back (densityFunction.n (intersectVerts[1], d));
      normals.push_back (densityFunction.n (intersectVerts[2], d));

      indices.push_back (startIndex);
      indices.push_back (startIndex+1);
      indices.push_back (startIndex+2);

      if (nofIntersectVerts == 4)
      {
        vertices.push_back (intersectVerts[3]);
        normals.push_back (densityFunction.n (intersectVerts[3], d));

        indices.push_back (startIndex+1);
        indices.push_back (startIndex+2);
        indices.push_back (startIndex+3);
      }
    }
  }

	gl::VboMesh::Layout layout;

  // Vbo settings
	layout.setStaticIndices ();
	layout.setStaticPositions ();
	layout.setStaticNormals ();

  // Create Vbo
  vboMesh = gl::VboMesh (vertices.size(), indices.size(), layout, GL_TRIANGLES);
	vboMesh.bufferIndices (indices);
	vboMesh.bufferPositions (vertices);
	vboMesh.bufferNormals (normals);
	vboMesh.unbindBuffers ();

  delete [] evals;
}


void IsoSurface::draw ()
{
  gl::draw (mVboMesh);

/*
  glColor3f (1,1,0);
  gl::drawSphere (Vec3f (-100, 0, 0), tmpRadius);

  glColor3f (1,0,0);
  gl::draw (mVboMesh);

  glColor3f (0,0,1);
  drawAllCubes ();
*/
}

void IsoSurface::setupTetraGrid (std::vector<uint32_t>& vboIndices, std::vector<Vec3f>& vboVertices, const Vec3f& GridSize, const Vec3i& GridResolution)
{
  Vec3f startPoint = GridSize / 2.0f;

  float xs = GridSize.x  / (float)GridResolution.x;
  float ys = GridSize.y  / (float)GridResolution.y;
  float zs = GridSize.z  / (float)GridResolution.z;

  // Create vertices
  for (uint32_t z=0; z<(uint32_t)GridResolution.z+1; z++)
  {
    for (uint32_t y=0; y<(uint32_t)GridResolution.y+1; y++)
    {
      for (uint32_t x=0; x<(uint32_t)GridResolution.x+1; x++)
      {
        Vec3f p = Vec3f (x*xs,y*ys,z*zs) + startPoint;
        vboVertices.push_back (p);
      }
    }
  }

  // Create indicies
  for (uint32_t z=0; z<(uint32_t)GridResolution.z; z++)
  {
    for (uint32_t y=0; y<(uint32_t)GridResolution.y; y++)
    {
      for (uint32_t x=0; x<(uint32_t)GridResolution.x; x++)
      {
        getTetraCubeIndices (x, y, z, GridResolution.x, GridResolution.y, vboIndices);
      }
    }
  }
}

void IsoSurface::setupTetraVbo ()
{

  Vec3f startPoint = Vec3f (-mGridWidth  / 2.0f, 
                            -mGridHeight / 2.0f, 
                            -mGridDepth  / 2.0f);

  float xs = mGridWidth  / (float)mGridNofX;
  float ys = mGridHeight / (float)mGridNofY;
  float zs = mGridDepth  / (float)mGridNofZ;

  // Create vertices
  for (uint32_t z=0; z<mGridNofZ+1; z++)
  {
    for (uint32_t y=0; y<mGridNofY+1; y++)
    {
      for (uint32_t x=0; x<mGridNofX+1; x++)
      {
        Vec3f p = Vec3f (x*xs,y*ys,z*zs) + startPoint;
        mVboVertices.push_back (p);
      }
    }
  }

  // Create indicies
  for (uint32_t z=0; z<mGridNofZ; z++)
  {
    for (uint32_t y=0; y<mGridNofY; y++)
    {
      for (uint32_t x=0; x<mGridNofX; x++)
      {
        getTetraCubeIndices (x, y, z, mGridNofX, mGridNofY, mVboIndices);
      }
    }
  }

	gl::VboMesh::Layout layout;
  // Vbo settings
	layout.setStaticIndices ();
	layout.setStaticPositions ();
  // Create Vbo
  mVboMesh = gl::VboMesh (mVboVertices.size(), mVboIndices.size(), layout, GL_LINES_ADJACENCY_EXT);
	mVboMesh.bufferIndices (mVboIndices);
	mVboMesh.bufferPositions (mVboVertices);
	mVboMesh.unbindBuffers ();	// Clean up

  /*
	mVboIndices.clear ();
	mVboVertices.clear ();
  */
}


void IsoSurface::getTetraCubeIndices (const uint32_t x, const uint32_t y, const uint32_t z, const uint32_t gridNofX, const uint32_t gridNofY,
                                               std::vector<uint32_t>& vboIndices)
{
  const uint32_t z_move = (gridNofX+1) * (gridNofY+1);
  const uint32_t y_move = (gridNofX+1);

  const uint32_t far_down_left    = z*z_move + y*y_move + x; // 0
  const uint32_t far_down_right   = far_down_left  + 1;      // 1
  const uint32_t far_up_right     = far_down_right + y_move; // 2
  const uint32_t far_up_left      = far_up_right   - 1;      // 3
  const uint32_t near_down_left   = far_down_left  + z_move; // 4
  const uint32_t near_down_right  = far_down_right + z_move; // 5
  const uint32_t near_up_right    = far_up_right   + z_move; // 6
  const uint32_t near_up_left     = far_up_left    + z_move; // 7

  // The 6 tetrahedrons that make up a cube
  const uint32_t tetraIndices[6][4] =
  {
    {near_up_right, near_down_left,   near_up_left,   far_down_left},
    {near_up_right, far_down_left,    near_up_left,   far_up_left},
    {near_up_right, far_down_left,    far_up_left,    far_up_right},
    {near_up_right, near_down_right,  near_down_left, far_down_left},
    {near_up_right, near_down_right,  far_down_left,  far_down_right},
    {near_up_right, far_up_right,     far_down_right, far_down_left}
  };

  for (uint32_t i=0; i<6; i++)
    for (uint32_t j=0; j<4; j++)
      vboIndices.push_back (tetraIndices[i][j]);
}

//   Linjärinterpolera för att hitta punkt på en linje där iso-värdet har sin gräns
Vec3f IsoSurface::VertexInterp (float isolevel, Vec3f p1, Vec3f p2, float valp1, float valp2)
{
  float dist = abs (valp2-valp1);

  float mu = (isolevel - valp1) / (valp2 - valp1);

  return lerp (p1, p2, mu);
}

// Använd extra punkt i mitten
Vec3f IsoSurface::VertexInterp (DensityInterface& densityFunction, float isolevel, Vec3f p1, Vec3f p2, float valp1, float valp2)
{
  return VertexInterp (isolevel, p1, p2, valp1, valp2);

//  if (abs (valp2-valp1) < 10.0f)
//    return VertexInterp (isolevel, p1, p2, valp1, valp2);

  Vec3f pm = (p1 + p2) / 2.0f;

  bool b1 = valp1 > isolevel;
  float valm = densityFunction.f (pm);
  bool bm = valm > isolevel;

  if (b1 != bm)
    return VertexInterp (isolevel, p1, pm, valp1, valm);
  else
    return VertexInterp (isolevel, pm, p2, valm, valp2);
}


void IsoSurface::getIntersection (DensityInterface& densityFunction, const Vec3f* verts, const float* evals, const uint32_t* indices, 
                      Vec3f* outVerts, uint32_t& nofVerts)
{
   const uint32_t i0 = indices[0];
   const uint32_t i1 = indices[1];
   const uint32_t i2 = indices[2];
   const uint32_t i3 = indices[3];

   const float e0 = evals[i0];
   const float e1 = evals[i1];
   const float e2 = evals[i2];
   const float e3 = evals[i3];

   const Vec3f p0 = verts[i0];
   const Vec3f p1 = verts[i1];
   const Vec3f p2 = verts[i2];
   const Vec3f p3 = verts[i3];

   uint32_t triIndex;

   // Determine which of the 16 cases we have given which vertices are above or below the isosurface
   triIndex = 0UL;
   if (e0 < isoLimit) 
     triIndex |= 1;
   if (e1 < isoLimit) 
     triIndex |= 2;
   if (e2 < isoLimit) 
     triIndex |= 4;
   if (e3 < isoLimit) 
     triIndex |= 8;

   // Form the vertices of the triangles for each case
   switch (triIndex) 
   {
     case 0x00:
     case 0x0F:
        nofVerts = 0;
        break;
     case 0x0E:
     case 0x01:
        outVerts[0] = VertexInterp (densityFunction, isoLimit, p0, p1, e0, e1);
        outVerts[1] = VertexInterp (densityFunction, isoLimit, p0, p2, e0, e2);
        outVerts[2] = VertexInterp (densityFunction, isoLimit, p0, p3, e0, e3);
        nofVerts = 3;
        break;
     case 0x0D:
     case 0x02:
        outVerts[0] = VertexInterp (densityFunction, isoLimit, p1, p0, e1, e0);
        outVerts[1] = VertexInterp (densityFunction, isoLimit, p1, p3, e1, e3);
        outVerts[2] = VertexInterp (densityFunction, isoLimit, p1, p2, e1, e2);
        nofVerts = 3;
        break;
     case 0x0C:
     case 0x03:
        outVerts[0] = VertexInterp (densityFunction, isoLimit, p0, p3, e0, e3);
        outVerts[1] = VertexInterp (densityFunction, isoLimit, p0, p2, e0, e2);
        outVerts[2] = VertexInterp (densityFunction, isoLimit, p1, p3, e1, e3);
        outVerts[3] = VertexInterp (densityFunction, isoLimit, p1, p2, e1, e2);
        nofVerts = 4;
        break;
     case 0x0B:
     case 0x04:
        outVerts[0] = VertexInterp (densityFunction, isoLimit, p2,  p0, e2, e0);
        outVerts[1] = VertexInterp (densityFunction, isoLimit, p2,  p1, e2, e1);
        outVerts[2] = VertexInterp (densityFunction, isoLimit, p2,  p3, e2, e3);
        nofVerts = 3;
        break;
     case 0x0A:
     case 0x05:
        outVerts[0] = VertexInterp (densityFunction, isoLimit, p0, p3, e0, e3);
        outVerts[1] = VertexInterp (densityFunction, isoLimit, p0, p1, e0, e1);
        outVerts[2] = VertexInterp (densityFunction, isoLimit, p2, p3, e2, e3);
        outVerts[3] = VertexInterp (densityFunction, isoLimit, p1, p2, e1, e2);
        nofVerts = 4;
        break;
     case 0x09:
     case 0x06:
        outVerts[0] = VertexInterp (densityFunction, isoLimit, p1, p3, e1, e3);
        outVerts[1] = VertexInterp (densityFunction, isoLimit, p0, p1, e0, e1);
        outVerts[2] = VertexInterp (densityFunction, isoLimit, p2, p3, e2, e3);
        outVerts[3] = VertexInterp (densityFunction, isoLimit, p0, p2, e0, e2);
        nofVerts = 4;
        break;
     case 0x07:
     case 0x08:
        outVerts[0] = VertexInterp (densityFunction, isoLimit, p3, p0, e3, e0);
        outVerts[1] = VertexInterp (densityFunction, isoLimit, p3, p2, e3, e2);
        outVerts[2] = VertexInterp (densityFunction, isoLimit, p3, p1, e3, e1);
        nofVerts = 3;
        break;
   }
}

#if 0
// Function declarations ///////////////////////////////////////////////////////////////////////////////////

float f (const Vec3f& p);

// Funciton implementations ////////////////////////////////////////////////////////////////////////////////
Vec3f fNormal (const Vec3f& p)
{
  Vec3f d1 = Vec3f (-100, 0, 0) - p;
//  Vec3f d2 = Vec3f ( 100, 0, 0) - p;

  Vec3f force = d1.normalized() / d1.lengthSquared () /*+ d2.normalized () / d2.lengthSquared ()*/;

  Vec3f ret = -force.normalized ();
  return ret;
}

const float tmpRadius = 80.0f;

float f (const Vec3f& p)
{
  Vec3f d1 = Vec3f (-100, 0, 0) - p;
//  Vec3f d2 = Vec3f ( 100, 0, 0) - p;

  float potential = 1.0f / d1.length () /*+ 1.0f / d2.lengthSquared ()*/;

  return tmpRadius*potential;
}
#endif

#if 0
void IsoSurface::drawAllCubes ()
{
  Vec3f startPoint = Vec3f (-mGridWidth  / 2.0f, 
                            -mGridHeight / 2.0f, 
                            -mGridDepth  / 2.0f);

  float xs = mGridWidth  / (float)mGridNofX;
  float ys = mGridHeight / (float)mGridNofY;
  float zs = mGridDepth  / (float)mGridNofZ;

  for (uint32_t x=0; x<mGridNofX; x++)
  {
    for (uint32_t y=0; y<mGridNofY; y++)
    {
      for (uint32_t z=0; z<mGridNofZ; z++)
      {
        Vec3f p = Vec3f (x*xs,y*ys,z*zs) + startPoint;

        drawTetraCube (p, xs);
      }
    }
  }
}

void IsoSurface::drawTetraCube (const Vec3f& pos, const float s)
{
  // cube corners
  const Vec3f cubeCorners[8] = 
  {
    Vec3f (0, 0, 0), // 0
    Vec3f (s, 0, 0), // 1
    Vec3f (s, s, 0), // 2
    Vec3f (0, s, 0), // 3
    Vec3f (0, 0, s), // 4
    Vec3f (s, 0, s), // 5
    Vec3f (s, s, s), // 6
    Vec3f (0, s, s)  // 7
  };

  // The 6 tetrahedrons that make up a cube
  static const uint32_t tetraIndices[6][4] =
  {
    {6, 4, 7, 0},
    {6, 0, 7, 3},
    {6, 0, 3, 2},
    {6, 5, 4, 0},
    {6, 5, 0, 1},
    {6, 2, 1, 0}
  };

  Vec3f verts[8];
  float evals[8];

  for (uint32_t i=0; i<8; i++)
    verts[i] = cubeCorners[i] + pos;

  for (uint32_t i=0; i<8; i++)
    evals[i] = f (verts[i]);

  for (uint32_t i=0; i<6; i++)
    drawTetrahedron (verts, evals, tetraIndices[i]);
}

void IsoSurface::drawTetrahedron (const Vec3f* verts, const float* evals, const uint32_t* indices)

{
  Vec3f vertices[4];
  uint32_t nofVertices;
   
  getIntersection (verts, evals, indices, vertices, nofVertices);

  glBegin (GL_TRIANGLE_STRIP);
    for (uint32_t i=0; i < nofVertices; i++)
    {
      Vec3f n = fNormal (vertices[i]);
      glNormal3fv (n);
      glVertex3fv (vertices[i]);
    }
  glEnd ();
}
#endif
