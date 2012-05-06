#version 120
#extension GL_EXT_geometry_shader4 : enable

const float isoLimit = 1.0;

uniform vec3  ballPositions[100];
uniform int   nofBalls;
uniform float radiusSquared;

varying in vec4 vPotential[4];
varying in vec3 vLightDir[4];
varying in vec3 vEyeVec[4];

varying out vec3 gNormal;
varying out vec3 gLightDir;
varying out vec3 gEyeVec;

// Must be the same in vertex shader
float potentialFunction (vec3 position)
{
  float potSum = 0;
  for (int i=0; i<nofBalls; i++)
  {
    vec3 dist           = ballPositions[i] - position;
    vec3 ballPotential  = -normalize (dist) / dot (dist, dist);
    potSum             += length (ballPotential);
  }

  return radiusSquared*potSum;
}

float getProportion (float v1, float v2)
{
  return (isoLimit - v1) / (v2 - v1);
}

float getProportionSpecial (float v1, float v2, vec3 pm)
{
  float vm = potentialFunction (pm); 

  bool b1 = v1 > isoLimit;
  bool bm = vm > isoLimit;

  if (b1 != bm)
    return 0.5 * getProportion (v1, vm);
  else
    return 0.5 + 0.5 * getProportion (vm, v2);
}


void VertexInterp (int i1, int i2)
{
//	float t = getProportion (vPotential[i1].w, vPotential[i2].w);  // Used to speed up but get worse interpolation
	float t = getProportionSpecial (vPotential[i1].w, vPotential[i2].w, (gl_PositionIn[i1].xyz + gl_PositionIn[i2].xyz)/2.0);

  // linearly interpolate position
  gl_Position = gl_ModelViewProjectionMatrix * mix(gl_PositionIn[i1], gl_PositionIn[i2],  t);
//  gl_Position = mix(gl_PositionIn[i1], gl_PositionIn[i2],  t);  // Used if transformation in vertex shader

  // linearly interpolate shader parameters
  gNormal     = mix(vPotential[i1].xyz, vPotential[i2].xyz, t);
  gLightDir   = mix(vLightDir[i1],      vLightDir[i2],      t);
  gEyeVec     = mix(vEyeVec[i1],        vEyeVec[i2],        t);

  // done with the vertex
  EmitVertex();
}


void main()
{
   int triIndex;

   // Determine which of the 16 cases we have given which vertices are above or below the isosurface
   triIndex = 0;
   if (vPotential[0].w < isoLimit) 
     triIndex |= 1;
   if (vPotential[1].w < isoLimit) 
     triIndex |= 2;
   if (vPotential[2].w < isoLimit) 
     triIndex |= 4;
   if (vPotential[3].w < isoLimit) 
     triIndex |= 8;

   // Form the vertices of the triangles for each case
   switch (triIndex) 
   {
     case 0x00:
     case 0x0F:
        break;
     case 0x0E:
     case 0x01:
        VertexInterp (0, 1);
        VertexInterp (0, 2);
        VertexInterp (0, 3);
        break;
     case 0x0D:
     case 0x02:
        VertexInterp (1, 0);
        VertexInterp (1, 3);
        VertexInterp (1, 2);
        break;
     case 0x0C:
     case 0x03:
        VertexInterp (0, 3);
        VertexInterp (0, 2);
        VertexInterp (1, 3);
        VertexInterp (1, 2);
        break;
     case 0x0B:
     case 0x04:
        VertexInterp (2, 0);
        VertexInterp (2, 1);
        VertexInterp (2, 3);
        break;
     case 0x0A:
     case 0x05:
        VertexInterp (0, 3);
        VertexInterp (0, 1);
        VertexInterp (2, 3);
        VertexInterp (2, 1);
        break;
     case 0x09:
     case 0x06:
        VertexInterp (1, 3);
        VertexInterp (0, 1);
        VertexInterp (2, 3);
        VertexInterp (2, 0);
        break;
     case 0x07:
     case 0x08:
        VertexInterp (3, 0);
        VertexInterp (3, 2);
        VertexInterp (3, 1);
        break;
   }

  EndPrimitive();
}
