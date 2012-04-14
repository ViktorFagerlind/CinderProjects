varying vec4 passColor;

attribute float size;

const float defaultScale = 1500.0;
const float minSize = 1.0;
const float maxSize = 100.0;

void main()
{
	gl_TexCoord[0] = gl_MultiTexCoord0;

  vec4 eyeCoord = gl_ModelViewMatrix * gl_Vertex;
  gl_Position = gl_ProjectionMatrix * eyeCoord;
  float dist = distance(eyeCoord, vec4(0.0, 0.0, 0.0, 1.0));

  passColor = gl_Color;

  gl_PointSize = clamp (defaultScale * size / dist, minSize, maxSize);
}
