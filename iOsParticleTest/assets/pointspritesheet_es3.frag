#version 300 es

uniform sampler2D uTex0;

in highp vec4	Color;
in highp vec2 TextureCoord;
in highp vec2 ImageSize;

out highp vec4 	oColor;

void main( void )
{
  mediump vec2 realTexCoord = TextureCoord + vec2 (gl_PointCoord.s*ImageSize.s, gl_PointCoord.t*ImageSize.t);
  
  oColor = texture (uTex0, realTexCoord) * Color;
}
