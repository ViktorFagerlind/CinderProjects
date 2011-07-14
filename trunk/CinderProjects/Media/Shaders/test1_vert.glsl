uniform float t;

void main()
{
	gl_TexCoord[0] = gl_MultiTexCoord0;

	gl_Vertex.x = gl_Vertex.x * (1.0 + 0.4 * sin (t*8.0));
	gl_Vertex.y = gl_Vertex.y * (1.0 + 0.4 * sin (t*5.0));
	gl_Vertex.z = gl_Vertex.z * (1.0 + 0.4 * sin (t*3.0));

	gl_Position = ftransform();
}
