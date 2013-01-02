#version 120
#extension GL_EXT_gpu_shader4 : require
#extension GL_EXT_geometry_shader4 : require

uniform vec3 u_point2;
uniform vec3 u_up1;
uniform vec3 u_up2;
uniform vec3 u_side1;
uniform vec3 u_side2;

varying in  vec3 vLightDir[1];
varying in  vec3 vEyeVec[1];

varying out vec3 gNormal;
varying out vec3 gLightDir;
varying out vec3 gEyeVec;

void createSide (vec4  world1, 
                 vec4  world2,
                 vec3  add1, 
                 vec3  add2,
                 float radius)
{
    vec4  world_add1, world_add2;

    world_add1  = gl_ModelViewProjectionMatrix * (vec4(add1 * radius,1));
    world_add2  = gl_ModelViewProjectionMatrix * (vec4(add2 * radius,1));

    gl_Position = world1 + world_add1;
    gNormal     = add1;
    EmitVertex();

    gl_Position = world2 + world_add2;
    gNormal     = add2;
    EmitVertex();
}

void drawCylinder (vec3 p1,           // end point 1
                   vec3 p2,           // end point 2
                   vec3 up1,          // "up" vector at point 1
                   vec3 up2,          // "up" vector at point 2
                   vec3 side1,        // "side" vector at point 1
                   vec3 side2,        // "side" vector at point 2
                   float radius,      // radius
                   int nofSegments)   // number of segments
{
  float PI = 3.14159f;
  vec3  e1, e2;

  vec4  world_p1 = gl_ModelViewProjectionMatrix * vec4(p1,1);
  vec4  world_p2 = gl_ModelViewProjectionMatrix * vec4(p2,1);

	for (int j=0; j<=nofSegments; j++) 
  {
    float theta1 = j * 2 * PI / nofSegments;

    vec3 w_up1   = up1 * cos (theta1);
    vec3 w_up2   = up2 * cos (theta1);
    vec3 w_side1 = side1 * sin (theta1);
    vec3 w_side2 = side2 * sin (theta1);

    e1 =  w_up1 + w_side1;
    e2 =  w_up2 + w_side2;
    createSide (world_p1, world_p2, e1, e2, radius);
  }
}

///////////////////////

void main()
{
  gLightDir = vLightDir[0];
  gEyeVec   = vEyeVec[0];

  drawCylinder (gl_PositionIn[0].xyz, 
                u_point2,
                u_up1,
                u_up2,
                u_side1,
                u_side2,
                2,
                8);
}


