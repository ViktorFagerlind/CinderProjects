#version 300 es

uniform mat4	ciModelViewProjection;

in vec4			ciPosition;
in vec4			ciColor;
in float  	vfSize;

out lowp vec4	Color;


void main( void )
{
	gl_Position	 = ciModelViewProjection * ciPosition;
  gl_PointSize = vfSize;
  
	Color 		   = ciColor;
}
