#include "March.h"

#include "Macros.h"
#include "cinder/CinderMath.h"

const uint32_t grid_nof_x = 60;
const uint32_t grid_nof_y = 60;
const uint32_t grid_nof_z = 60;

const float grid_width  = 500.0f;
const float grid_height = 500.0f;
const float grid_depth  = 500.0f;

const float isoLimit = 0.00015f;

// Function declarations ///////////////////////////////////////////////////////////////////////////////////
void drawTetraCube (const Vec3f& pos, const float s);

void drawTetrahedron (const Vec3f* verts, const float* evals, const uint8_t* indices);

void getIntersection (const Vec3f* verts, const float* evals, const uint8_t* indices, 
                      Vec3f* outVerts, uint32_t& nofVerts);

float f (const Vec3f& p);

// Funciton implementations ////////////////////////////////////////////////////////////////////////////////
Vec3f fNormal (const Vec3f& p)
{
  Vec3f d1 = Vec3f (-100, 0, 0) - p;
  Vec3f d2 = Vec3f ( 100, 0, 0) - p;

  Vec3f force = d1.normalized() / d1.lengthSquared () + d2.normalized () / d2.lengthSquared ();

  Vec3f ret = -force.normalized ();
  return ret;
}


float f (const Vec3f& p)
{
  Vec3f d1 = Vec3f (-100, 0, 0) - p;
  Vec3f d2 = Vec3f ( 100, 0, 0) - p;

  float potential = 1.0f / d1.lengthSquared () + 1.0f / d2.lengthSquared ();

  return potential;
}

void March::draw ()
{
  Vec3f startPoint = Vec3f (-grid_width  / 2.0f, 
                            -grid_height / 2.0f, 
                            -grid_depth  / 2.0f);

  float xs = grid_width  / (float)grid_nof_x;
  float ys = grid_height / (float)grid_nof_y;
  float zs = grid_depth  / (float)grid_nof_z;

  for (int x=0; x<grid_nof_x; x++)
  {
    for (int y=0; y<grid_nof_y; y++)
    {
      for (int z=0; z<grid_nof_z; z++)
      {
        Vec3f p = Vec3f (x*xs,y*ys,z*zs) + startPoint;

        drawTetraCube (p, xs);
      }
    }
  }
}

void drawTetraCube (const Vec3f& pos, const float s)
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
  static const uint8_t tetraIndices[6][4] =
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

void drawTetrahedron (const Vec3f* verts, const float* evals, const uint8_t* indices)

{
  gl::enableDepthRead ();

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


//   Linjärinterpolera för att hitta punkt på en linje där iso-värdet har sin gräns
Vec3f VertexInterp (float isolevel, Vec3f p1, Vec3f p2, float valp1, float valp2)
{
  float mu;
  Vec3f p;

  if (abs(isolevel-valp1) < 0.0000001)
    return(p1);
  if (abs(isolevel-valp2) < 0.0000001)
    return(p2);
  if (abs(valp1-valp2) < 0.0000001)
    return(p1);
  mu = (isolevel - valp1) / (valp2 - valp1);
  p.x = p1.x + mu * (p2.x - p1.x);
  p.y = p1.y + mu * (p2.y - p1.y);
  p.z = p1.z + mu * (p2.z - p1.z);

  return(p);
}

void getIntersection (const Vec3f* verts, const float* evals, const uint8_t* indices, 
                      Vec3f* outVerts, uint32_t& nofVerts)
{
   const uint8_t i0 = indices[0];
   const uint8_t i1 = indices[1];
   const uint8_t i2 = indices[2];
   const uint8_t i3 = indices[3];

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
        outVerts[0] = VertexInterp (isoLimit, p0, p1, e0, e1);
        outVerts[1] = VertexInterp (isoLimit, p0, p2, e0, e2);
        outVerts[2] = VertexInterp (isoLimit, p0, p3, e0, e3);
        nofVerts = 3;
        break;
     case 0x0D:
     case 0x02:
        outVerts[0] = VertexInterp (isoLimit, p1, p0, e1, e0);
        outVerts[1] = VertexInterp (isoLimit, p1, p3, e1, e3);
        outVerts[2] = VertexInterp (isoLimit, p1, p2, e1, e2);
        nofVerts = 3;
        break;
     case 0x0C:
     case 0x03:
        outVerts[0] = VertexInterp (isoLimit, p0, p3, e0, e3);
        outVerts[1] = VertexInterp (isoLimit, p0, p2, e0, e2);
        outVerts[2] = VertexInterp (isoLimit, p1, p3, e1, e3);
        outVerts[3] = VertexInterp (isoLimit, p1, p2, e1, e2);
        nofVerts = 4;
        break;
     case 0x0B:
     case 0x04:
        outVerts[0] = VertexInterp (isoLimit, p2,  p0, e2, e0);
        outVerts[1] = VertexInterp (isoLimit, p2,  p1, e2, e1);
        outVerts[2] = VertexInterp (isoLimit, p2,  p3, e2, e3);
        nofVerts = 3;
        break;
     case 0x0A:
     case 0x05:
        outVerts[0] = VertexInterp (isoLimit, p0, p3, e0, e3);
        outVerts[1] = VertexInterp (isoLimit, p0, p1, e0, e1);
        outVerts[2] = VertexInterp (isoLimit, p2, p3, e2, e3);
        outVerts[3] = VertexInterp (isoLimit, p1, p2, e1, e2);
        nofVerts = 4;
        break;
     case 0x09:
     case 0x06:
        outVerts[0] = VertexInterp (isoLimit, p1, p3, e1, e3);
        outVerts[1] = VertexInterp (isoLimit, p0, p1, e0, e1);
        outVerts[2] = VertexInterp (isoLimit, p2, p3, e2, e3);
        outVerts[3] = VertexInterp (isoLimit, p0, p2, e0, e2);
        nofVerts = 4;
        break;
     case 0x07:
     case 0x08:
        outVerts[0] = VertexInterp (isoLimit, p3, p0, e3, e0);
        outVerts[1] = VertexInterp (isoLimit, p3, p2, e3, e2);
        outVerts[2] = VertexInterp (isoLimit, p3, p1, e3, e1);
        nofVerts = 3;
        break;
   }
}
