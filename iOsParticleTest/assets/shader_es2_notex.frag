#version 100

precision highp float;

//uniform sampler2D 		uTex0;

varying highp vec4    Color;
//varying highp vec2		TexCoord0;
varying highp vec3		Normal;

void main( void )
{
  highp vec3 normal = normalize( -Normal );
  highp float diffuse = max( dot( normal, normalize(vec3( 10, -6, -15 ) )), 0.0 );
  gl_FragColor = Color * diffuse;
}