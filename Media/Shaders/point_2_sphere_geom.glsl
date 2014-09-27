#version 120
#extension GL_EXT_gpu_shader4 : require
#extension GL_EXT_geometry_shader4 : require

varying in  vec3 vLightDir[1];
varying in  vec3 vEyeVec[1];

varying out vec3 gNormal;
varying out vec3 gLightDir;
varying out vec3 gEyeVec;

void drawSphere (float radius, float segments)
{
  float PI = 3.14159f;
  vec3  e;
  vec3  position;

	for (int j=0; j<segments/2; j++) 
  {
		float theta1 = j * 2 * PI / segments - ( PI / 2.0f );
		float theta2 = (j + 1) * 2 * PI / segments - ( PI / 2.0f );

		for( int i = 0; i <= segments; i++ ) 
    {
			float theta3 = i * 2 * PI / segments;

			e.x = cos( theta1 ) * cos( theta3 );
			e.y = sin( theta1 );
			e.z = cos( theta1 ) * sin( theta3 );

      position    = gl_PositionIn[0].xyz + e * radius;
      gl_Position = gl_ModelViewProjectionMatrix * vec4(position,1);
      gNormal     = e;

      EmitVertex();

			e.x = cos( theta2 ) * cos( theta3 );
			e.y = sin( theta2 );
			e.z = cos( theta2 ) * sin( theta3 );

      position    = gl_PositionIn[0].xyz + e * radius;
      gl_Position = gl_ModelViewProjectionMatrix * vec4(position,1);
      gNormal     = e;

      EmitVertex();
		}

    EndPrimitive();
	}

}

///////////////////////

void main()
{
  gLightDir = vLightDir[0];
  gEyeVec   = vEyeVec[0];

  drawSphere (0.6, 8);
}
