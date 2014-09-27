#version 120

varying vec3 lightDir;
varying vec3 eyeVec;
varying vec3 normal;

void main()
{
	normal = normalize(gl_NormalMatrix * gl_Normal);

	vec3 vertex = vec3(gl_ModelViewMatrix * gl_Vertex);
	lightDir = vec3(gl_LightSource[0].position.xyz - vertex);
	eyeVec = -vertex;

	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = ftransform();
}
