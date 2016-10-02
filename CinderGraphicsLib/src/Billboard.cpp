#include "Billboard.h"

#include "cinder/gl/gl.h"

void Billboard::drawBillboardTex (const vec3   &pos,
                                  const vec2   &scale,
                                  const float *texCoords,
                                  const ColorAf &color,
                                  const float   rotationDegrees,
                                  const vec3   &bbRight,
                                  const vec3   &bbUp)
{
  vec3   verts[4];
  ColorAf colors[4] = { color, color, color, color };

  glVertexPointer (3, GL_FLOAT, 0, verts);
  glTexCoordPointer (2, GL_FLOAT, 0, texCoords);
  glColorPointer (4, GL_FLOAT, 0, colors);

  float sinA = math<float>::sin (rotationDegrees);
  float cosA = math<float>::cos (rotationDegrees);

  verts[0] = pos + bbRight * (-1.f * scale.x * cosA - 1.f * sinA * scale.y) + bbUp * (-1.f * scale.x * sinA + 1.f * cosA * scale.y);
  verts[1] = pos + bbRight * (-1.f * scale.x * cosA - -1.f * sinA * scale.y) + bbUp * (-1.f * scale.x * sinA + -1.f * cosA * scale.y);
  verts[2] = pos + bbRight * (1.f * scale.x * cosA - 1.f * sinA * scale.y) + bbUp * (1.f * scale.x * sinA + 1.f * cosA * scale.y);
  verts[3] = pos + bbRight * (1.f * scale.x * cosA - -1.f * sinA * scale.y) + bbUp * (1.f * scale.x * sinA + -1.f * cosA * scale.y);

  glDrawArrays (GL_TRIANGLE_STRIP, 0, 4);
}

void Billboard::drawBillboard (const vec3   &pos,
                               const vec2   &scale,
                               const ColorAf &color,
                               const float   rotationDegrees,
                               const vec3   &bbRight,
                               const vec3   &bbUp)
{
  GLfloat texCoords[8] = { 0, 0, 0, 1, 1, 0, 1, 1 };

  drawBillboardTex (pos, scale, texCoords, color, rotationDegrees, bbRight, bbUp);
}
