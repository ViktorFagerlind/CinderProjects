varying vec4 passColor;
uniform sampler2D texUnit;

void main()
{
//  gl_FragColor = texture2D(0, gl_TexCoord[0]);

	//Apply values from texture unit 0
	vec4 texel = texture2D( texUnit, gl_TexCoord[0].st );
	
  gl_Color = vec4(1,1,1,1);
	//Fragment color is the average of the texel and value of gl_Color, but using the texels original alpha
	gl_FragColor = passColor * texel;
}
