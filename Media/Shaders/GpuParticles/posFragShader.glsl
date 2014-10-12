#version 120 

uniform sampler2D posTex;
uniform sampler2D velTex;
uniform sampler2D infTex;

uniform sampler2D spriteTex;

varying float age;

void main()
{
	vec4 colFac;// = texture2D(spriteTex, gl_PointCoord);

    colFac.rgb = texture2D (infTex, gl_TexCoord[0].st).rgb;

	colFac.rgb += vec3(0.2, 0.2, 0.2);
	
	colFac.a = 1.0 - age;
    
	gl_FragColor = colFac;
}
