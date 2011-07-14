uniform float t;

float plasma_base(in float x, in float y) 
{
	return 0.25 + sin(x / (0.1 * cos(y * 0.8))) * 1.0 * cos(y / (0.1 * sin(x*0.5)));
}

void main()
{
	float x = gl_TexCoord[0].s;
	float y = gl_TexCoord[0].t;

	float aa = sin (10.0*(x+t));
	float bb = sin (10.0*(y+t));
	float cc = sin (10.0*(x*y+t));

	float r = plasma_base (x + cos (t), y + sin (t*0.9));
	float g = plasma_base (x * cos (t*0.3), y + sin (t*1.2));
	float b = plasma_base (x + cos (t), y * sin (t*0.1));

	vec3 color = vec3 (2.0*r+g+0.5*b, r+g+b, 0.5*r+g+2.0*b);

	gl_FragColor.rgb = color;
	gl_FragColor.a = 1.0;
}