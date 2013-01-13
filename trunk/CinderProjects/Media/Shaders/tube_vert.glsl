#version 120

varying vec3 vLightDir;
varying vec3 vEyeVec;


varying vec3  v_point2;
varying vec3  v_planeNormal1;
varying vec3  v_planeNormal2;
varying float v_radius1;
varying float v_radius2;

/*
uniform vec3  a_point2;
uniform vec3  a_planeNormal1;
uniform vec3  a_planeNormal2;
uniform float a_radius1;
uniform float a_radius2;
*/

attribute vec3  a_point2;
attribute vec3  a_planeNormal1;
attribute vec3  a_planeNormal2;
attribute float a_radius1;
attribute float a_radius2;

void main()
{
  v_point2        = a_point2;
  v_planeNormal1  = a_planeNormal1;
  v_planeNormal2  = a_planeNormal2;
  v_radius1       = a_radius1;
  v_radius2       = a_radius2;

	vec3 vertex     = vec3(gl_ModelViewMatrix * gl_Vertex);
	vLightDir       = vec3(gl_LightSource[0].position.xyz - vertex);
	vEyeVec         = -vertex;

	gl_TexCoord[0]  = gl_MultiTexCoord0;


//  gl_Position   = gl_ModelViewProjectionMatrix * gl_Vertex; // Used if transformation in vertex shader
  gl_Position     = gl_Vertex;
}
