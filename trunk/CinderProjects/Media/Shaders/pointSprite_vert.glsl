varying vec4 passColor;

attribute float size;

void main()
{
  passColor = gl_Color;

  gl_PointSize = size;

	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = ftransform();
}
