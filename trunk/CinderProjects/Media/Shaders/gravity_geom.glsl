#version 120 
#extension GL_EXT_geometry_shader4 : enable
 
uniform float elementSize;
uniform int nofElementsPerSide;

void main(void)
{
  vec4 center = gl_PositionIn[0];

  float halfFieldSize = -(nofElementsPerSide * elementSize) / 2;

	for (int ey = 0; ey < nofElementsPerSide+1; ey++)
	{
    float y = halfFieldSize + ey * elementSize;

	  for (int ex = 0; ex < nofElementsPerSide+1; ex++)
	  {
      float x = halfFieldSize + ex * elementSize;

		  gl_Position = center + vec4(x, y, 0, 0);
		  EmitVertex();
	  }
  	EndPrimitive();
  }

	for (int ex = 0; ex < nofElementsPerSide+1; ex++)
	{
    float x = halfFieldSize + ex * elementSize;

  	for (int ey = 0; ey < nofElementsPerSide+1; ey++)
	  {
      float y = halfFieldSize + ey * elementSize;

		  gl_Position = center + vec4(x, y, 0, 0);
		  EmitVertex();
	  }
  	EndPrimitive();
  }
}
