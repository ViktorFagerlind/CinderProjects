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


void createSide (vec4  world_p1, 
                 vec4  world_p2,
                 vec3  e11, 
                 vec3  e12,
                 vec3  e21, 
                 vec3  e22,
                 float radius)
{
    vec4  world_e11, world_e12, world_e21, world_e22;

    world_e11 = gl_ModelViewProjectionMatrix * (vec4(e11 * radius,1));
    world_e12 = gl_ModelViewProjectionMatrix * (vec4(e12 * radius,1));
    world_e21 = gl_ModelViewProjectionMatrix * (vec4(e21 * radius,1));
    world_e22 = gl_ModelViewProjectionMatrix * (vec4(e22 * radius,1));

    gl_Position = world_p1 + world_e11;
    gNormal  = e11;
    EmitVertex();

    gl_Position = world_p1 + world_e12;
    gNormal  = e12;
    EmitVertex();

    gl_Position = world_p2 + world_e21;
    gNormal  = e21;
    EmitVertex();

    gl_Position = world_p2 + world_e22;
    gNormal  = e22;
    EmitVertex();

    EndPrimitive();
}

void drawCylinder (vec3 p1,           // end point 1
                   vec3 p2,           // end point 2
                   vec3 up,           // common "up" vector
                   vec3 side1,        // "side" vector at point 1
                   vec3 side2,        // "side" vector at point 2
                   float radius,      // radius
                   int nofSegments)   // number of segments
{
  float PI = 3.14159f;
  vec3  e11, e12, e21, e22;

  vec4  world_p1 = gl_ModelViewProjectionMatrix * vec4(p1,1);
  vec4  world_p2 = gl_ModelViewProjectionMatrix * vec4(p2,1);

	for (int j=0; j<nofSegments/4; j++) 
  {
    float theta1 = j     * 2 * PI / nofSegments;
    float theta2 = (j+1) * 2 * PI / nofSegments;

    vec3 up1 = up * cos (theta1);
    vec3 up2 = up * cos (theta2);

    vec3 side11 = side1 * sin (theta1);
    vec3 side12 = side1 * sin (theta2);
    vec3 side21 = side2 * sin (theta1);
    vec3 side22 = side2 * sin (theta2);

    e11 =  up1 + side11;
    e12 =  up2 + side12;
    e21 =  up1 + side21;
    e22 =  up2 + side22;
    createSide (world_p1, world_p2, e11, e12, e21, e22, radius);

    e11 =  up1 - side11;
    e12 =  up2 - side12;
    e21 =  up1 - side21;
    e22 =  up2 - side22;
    createSide (world_p1, world_p2, e11, e12, e21, e22, radius);

    e11 = -up1 - side11;
    e12 = -up2 - side12;
    e21 = -up1 - side21;
    e22 = -up2 - side22;
    createSide (world_p1, world_p2, e11, e12, e21, e22, radius);

    e11 = -up1 + side11;
    e12 = -up2 + side12;
    e21 = -up1 + side21;
    e22 = -up2 + side22;
    createSide (world_p1, world_p2, e11, e12, e21, e22, radius);
  }
}

///////////////////////

void main()
{
  gLightDir = vLightDir[0];
  gEyeVec   = vEyeVec[0];

  drawCylinder (gl_PositionIn[0].xyz - vec3(13,0,0), 
                gl_PositionIn[0].xyz + vec3(13,0,0), 
                vec3(   0,   0, 1),
                vec3(-0.7, 0.7, 0),
                vec3( 0.7, 0.7, 0),
                10,
                8);
/*
  drawCylinder (gl_PositionIn[0].xyz - vec3(0,13,0), 
                gl_PositionIn[0].xyz + vec3(0,13,0), 
                vec3(1,0,0),
                vec3(0,0,1),
                10,
                8);
*/
}


