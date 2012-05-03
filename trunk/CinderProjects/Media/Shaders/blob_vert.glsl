#version 120

uniform vec3 ballPositions[100];
uniform int  nofBalls;

varying vec3 vLightDir;
varying vec3 vEyeVec;
varying vec4 vPotential;

vec4 potentialFunction ()
{
  vec4 pot = vec4(0,0,0,0);
  for (int i=0; i<nofBalls; i++)
  {
    vec3 dist           = ballPositions[i] - gl_Vertex.xyz;
    vec3 ballPotential  = -normalize (dist) / dot (dist, dist);
    pot.xyz     += ballPotential;
    pot.w       += length (ballPotential);
  }

  return pot;
}

void main()
{	
  vPotential = potentialFunction ();

  // calculate light and eye positions and pass them to fragment shader
	vec3 vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);
	vLightDir = vec3(gl_LightSource[0].position.xyz - vVertex);
	vEyeVec = -vVertex;

  // calculate and pass normal to fragment shader 
	vPotential.xyz = gl_NormalMatrix * vPotential.xyz;
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}