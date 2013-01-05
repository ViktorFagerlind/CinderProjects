#version 120
#extension GL_EXT_gpu_shader4 : require
#extension GL_EXT_geometry_shader4 : require


uniform vec3  u_generalUp;
uniform vec3  u_point2;
uniform vec3  u_planeNormal1;
uniform vec3  u_planeNormal2;
uniform float u_radius1;
uniform float u_radius2;

varying in  vec3 vLightDir[1];
varying in  vec3 vEyeVec[1];

varying out vec3 normal;
varying out vec3 lightDir;
varying out vec3 eyeVec;

void drawCylinder (vec3 p1,           // end point 1
                   vec3 p2,           // end point 2
                   vec3 up1,          // "up" vector at point 1
                   vec3 up2,          // "up" vector at point 2
                   vec3 side1,        // "side" vector at point 1
                   vec3 side2,        // "side" vector at point 2
                   float radius1,     // radius at point 1
                   float radius2,     // radius at point 2
                   int nofSegments)   // number of segments
{
  float PI = 3.14159f;
  vec3  e1, e2;

	for (int j=0; j<=nofSegments; j++) 
  {
    float theta1 = j * 2 * PI / nofSegments;

    vec3 w_up1   = up1 * cos (theta1);
    vec3 w_up2   = up2 * cos (theta1);
    vec3 w_side1 = side1 * sin (theta1);
    vec3 w_side2 = side2 * sin (theta1);

    e1 =  w_up1 + w_side1;
    e2 =  w_up2 + w_side2;

    gl_Position = gl_ModelViewProjectionMatrix * vec4(p1 + e1 * radius1,1);
    normal     = e1;
    EmitVertex();

    gl_Position = gl_ModelViewProjectionMatrix * vec4(p2 + e2 * radius2,1);
    normal     = e2;
    EmitVertex();
  }
}

// Projects v onto a plane with normal n
// n must be of unity length
vec3 projectOnPlane (vec3 n, const vec3 v)
{
  return v - n * dot (n, v);
}

///////////////////////

void main()
{
  lightDir = vLightDir[0];
  eyeVec   = vEyeVec[0];

  vec3 up1, up2, side1, side2;

  up1   = normalize (projectOnPlane (u_planeNormal1, u_generalUp));
  side1 = normalize (cross (u_planeNormal1, up1));

  up2   = normalize (projectOnPlane (u_planeNormal2, u_generalUp));
  side2 = normalize (cross (u_planeNormal2, up2));

  drawCylinder (gl_PositionIn[0].xyz, 
                u_point2,
                up1,
                up2,
                side1,
                side2,
                u_radius1,
                u_radius2,
                12);

  EndPrimitive ();
}


