#version 120

uniform sampler2D tex0;

varying vec3 normal;
varying vec3 lightDir;
varying vec3 eyeVec;

void main()
{	
  float PI = 3.14159f;

	vec4 final_color = (gl_FrontLightModelProduct.sceneColor * gl_FrontMaterial.ambient) + 
	                   (gl_LightSource[0].ambient * gl_FrontMaterial.ambient);
							
  vec3 E = normalize(eyeVec);
	vec3 N = normalize(normal);
	vec3 L = normalize(lightDir);
	
	float eyeTerm = cos (acos (dot(N,E)) - PI/2.0);

	if(eyeTerm > 0.0)
	{
		final_color += gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse * eyeTerm * eyeTerm;	
  }

	float lambertTerm = dot(N,L);
	
	if(lambertTerm > 0.0)
	{
		vec3 R = reflect(-L, N);
		float specular = pow( max(dot(R, E), 0.0), gl_FrontMaterial.shininess );
		final_color += gl_LightSource[0].specular * gl_FrontMaterial.specular * specular;	
	}

	gl_FragColor   = final_color;
  gl_FragColor.a = 0.5;
}
