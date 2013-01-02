#version 120

uniform sampler2D tex0;

varying vec3 gNormal;
varying vec3 gLightDir;
varying vec3 gEyeVec;

void main()
{	
	vec4 final_color = (gl_FrontLightModelProduct.sceneColor * gl_FrontMaterial.ambient) + 
	                   (gl_LightSource[0].ambient * gl_FrontMaterial.ambient);
							
	vec3 N = normalize(gNormal);
	vec3 L = normalize(gLightDir);
	
	float lambertTerm = dot(N,L);
	
	if(lambertTerm > 0.0)
	{
		final_color += gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse * lambertTerm;	
		
		vec3 E = normalize(gEyeVec);
		vec3 R = reflect(-L, N);
		float specular = pow( max(dot(R, E), 0.0), gl_FrontMaterial.shininess );
		final_color += gl_LightSource[0].specular * gl_FrontMaterial.specular * specular;	
	}

	gl_FragColor = final_color + vec4(0.2, 0.2, 0.2, 1.0);
}