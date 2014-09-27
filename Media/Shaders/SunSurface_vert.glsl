uniform float t;

varying vec4 tmp;

void main()
{
	gl_TexCoord[0] = gl_MultiTexCoord0;

	tmp = gl_Vertex;
	tmp.x = tmp.x * (1.0 + 0.2 * sin (t*5.0));
	tmp.y = tmp.y * (1.0 + 0.2 * sin (t*8.0));
	tmp.z = tmp.z * (1.0 + 0.2 * sin (t*10.0));

	gl_Position = gl_ModelViewProjectionMatrix * tmp;
}
