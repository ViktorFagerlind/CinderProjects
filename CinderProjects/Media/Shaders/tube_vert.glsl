#version 120

varying vec3 vLightDir;
varying vec3 vEyeVec;

void main()
{
	vec3 vertex = vec3(gl_ModelViewMatrix * gl_Vertex);
	vLightDir = vec3(gl_LightSource[0].position.xyz - vertex);
	vEyeVec = -vertex;

	gl_TexCoord[0] = gl_MultiTexCoord0;


//  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex; // Used if transformation in vertex shader
  gl_Position = gl_Vertex;
}
