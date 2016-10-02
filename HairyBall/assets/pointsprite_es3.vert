#version 300 es

uniform mat4	ciModelViewProjection;

in vec4			ciPosition;
in vec4			ciColor;
in float  	vfSize;
in float    vfLife;

out lowp vec4	Color;


void main( void )
{
	gl_Position	 = ciModelViewProjection * ciPosition;
  gl_PointSize = vfSize;
  
  if (vfLife <= 0.0)
    Color = vec4(0);
  else
  	Color = ciColor;
}
