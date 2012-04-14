uniform vec3  elementSize;
uniform vec3  massPositions[100];
uniform float masses[100];
uniform float radius[100];
uniform int   nofMasses;

varying float intensity;

const float displaceFactor  = 0.0002;
const float colorFactor     = 0.02;

void main(void)
{
  // Forward texture coordinate
	gl_TexCoord[0] = gl_MultiTexCoord0;

	vec4 vert = gl_Vertex;
  vec3 positionChange = vec3 (0,0,0);


  for (int i=0; i<nofMasses; i++)
  {
    float m = masses[i];
    float r = radius[i];

    vec3 distanceVector = massPositions[i] - gl_Vertex.xyz;
    float distance = length (distanceVector);

    float maxStrength = m / (r * r);
    float strength = clamp (m / (distance*distance), 0.0, maxStrength);

    positionChange += displaceFactor * strength * normalize (distanceVector);
  }

  // Calculate color
  intensity = clamp (colorFactor * length (positionChange), 0.0, 1.0);

  // Clamp and affect position
  vert.x += clamp (positionChange.x, -elementSize.x, elementSize.x);
  vert.y += clamp (positionChange.y, -elementSize.y, elementSize.y);
  vert.z += clamp (positionChange.z, -elementSize.z, elementSize.z);

  // Transform position
	gl_Position = gl_ModelViewProjectionMatrix * vert;
}
