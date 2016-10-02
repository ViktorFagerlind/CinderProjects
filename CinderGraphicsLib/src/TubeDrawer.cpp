#include "TubeDrawer.h"

#include "MiscMath.h"

void TubeDrawer::draw (gl::GlslProg&   shader, 
                       const vec3&    upDirection,
                       const bool      blendFirst,
                       const uint32_t  nofCircularSegments,
                             vec3    *points,
                             vec3    *normals,
                             float    *radie,
                       const uint32_t  nofPoints)
{
  shader.uniform ("u_generalUp",   upDirection);
  shader.uniform ("u_nofSegments", (int)nofCircularSegments);

  GLint location[5];

  location[0] = shader.getAttribLocation ("a_point2");
  glEnableVertexAttribArrayARB (location[0]);
  glVertexAttribPointerARB (location[0], 3, GL_FLOAT, GL_FALSE, 0, &points[1]);

  location[1] = shader.getAttribLocation ("a_planeNormal1");
  glEnableVertexAttribArrayARB (location[1]);
  glVertexAttribPointerARB (location[1], 3, GL_FLOAT, GL_FALSE, 0, &normals[0]);

  location[2] = shader.getAttribLocation ("a_planeNormal2");
  glEnableVertexAttribArrayARB (location[2]);
  glVertexAttribPointerARB (location[2], 3, GL_FLOAT, GL_FALSE, 0, &normals[1]);

  location[3] = shader.getAttribLocation ("a_radius1");
  glEnableVertexAttribArrayARB (location[3]);
  glVertexAttribPointerARB (location[3], 1, GL_FLOAT, GL_FALSE, 0, &radie[0]);

  location[4] = shader.getAttribLocation ("a_radius2");
  glEnableVertexAttribArrayARB (location[4]);
  glVertexAttribPointerARB (location[4], 1, GL_FLOAT, GL_FALSE, 0, &radie[1]);

  glVertexPointer (3, GL_FLOAT, 0, points);

  glEnableClientState (GL_VERTEX_ARRAY);		

  if (blendFirst)
  {
    shader.uniform ("u_blendSphere", true);
    glDrawArrays (GL_POINTS, 0, 1);
    shader.uniform ("u_blendSphere", false);
    glDrawArrays (GL_POINTS, 1, nofPoints-2);
  }
  else
  {
    shader.uniform ("u_blendSphere", false);
    glDrawArrays (GL_POINTS, 0, nofPoints-1);
  }

	glDisableClientState (GL_VERTEX_ARRAY);

  for (uint32_t i=0; i<5; i++)
    glDisableVertexAttribArrayARB (location[i]);
}


void TubeDrawer::debugDraw (const vec3&     upDirection,
                            vec3           *points,
                            vec3           *normals,
                            float           *radie,
                            const uint32_t   nofPoints)
{
  for (uint32_t i=0; i<nofPoints - 1; i++)
  {
    vec3 up   = MiscMath::projectOnPlane (normals[i], upDirection).normalized ();
    vec3 side = normals[i].cross (up).normalized();

    gl::color (1,0,0);
    gl::drawLine (points[i], points[i] + normals[i]);
    gl::color (0,1,0);
    gl::drawLine (points[i], points[i] + up);
    gl::color (0,0,1);
    gl::drawLine (points[i], points[i] + side);
  }

}