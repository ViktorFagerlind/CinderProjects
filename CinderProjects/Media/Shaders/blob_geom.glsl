#version 120
#extension GL_EXT_geometry_shader4 : enable

const float isoLimit = 0.00055;

varying in vec4 vPotential[4];
varying in vec3 vLightDir[4];
varying in vec3 vEyeVec[4];

varying out vec3 gNormal;
varying out vec3 gLightDir;
varying out vec3 gEyeVec;

void VertexInterp (int i1, int i2)
{
	float t = (isoLimit - vPotential[i1].w) / (vPotential[i2].w - vPotential[i1].w);

  // linearly interpolate position
  gl_Position = mix(gl_PositionIn[i1],  gl_PositionIn[i2],  t);

  // linearly interpolate shader parameters
  gNormal     = mix(vPotential[i1].xyz, vPotential[i2].xyz, t);
  gLightDir   = mix(vLightDir[i1],      vLightDir[i2],      t);
  gEyeVec     = mix(vEyeVec[i1],        vEyeVec[i2],        t);

  // done with the vertex
  EmitVertex();
}


void main()
{
   float e0 = vPotential[0].w;
   float e1 = vPotential[1].w;
   float e2 = vPotential[2].w;
   float e3 = vPotential[3].w;

   vec4 p0 = gl_PositionIn[0];
   vec4 p1 = gl_PositionIn[1];
   vec4 p2 = gl_PositionIn[2];
   vec4 p3 = gl_PositionIn[3];

   int triIndex;

   // Determine which of the 16 cases we have given which vertices are above or below the isosurface
   triIndex = 0;
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
        VertexInterp (1, 2);
        break;
     case 0x09:
     case 0x06:
        VertexInterp (1, 3);
        VertexInterp (0, 1);
        VertexInterp (2, 3);
        VertexInterp (0, 2);
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
