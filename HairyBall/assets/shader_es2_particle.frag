#version 100

precision highp float;

uniform sampler2D 		uTex0;

varying highp vec4    Color;
varying highp vec2		TexCoord0;
//varying highp vec3		Normal;

void main( void )
{
  gl_FragColor = texture2D( uTex0, TexCoord0.st ) * Color;
}