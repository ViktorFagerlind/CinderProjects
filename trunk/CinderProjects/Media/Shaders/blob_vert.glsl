#version 120

uniform vec3  ballPositions[100];
uniform int   nofBalls;
uniform float radiusSquared;

varying vec3 vLightDir;
varying vec3 vEyeVec;
varying vec4 vPotential;

vec4 potentialFunction ()
{
  vec3  normalSum = vec3(0,0,0);
  float potSum;
  for (int i=0; i<nofBalls; i++)
  {
    vec3 dist           = ballPositions[i] - gl_Vertex.xyz;
    vec3 ballPotential  = -normalize (dist) / dot (dist, dist);
    normalSum          += ballPotential;
    potSum             += length (ballPotential);
  }

  vec4 pot;
  pot.xyz = normalize (normalSum);
  pot.w   = radiusSquared*potSum;

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