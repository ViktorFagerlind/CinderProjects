#include "NewParticleDrawer.h"

#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace cinder;

NewParticleDrawer::NewParticleDrawer ()
{
}

NewParticleDrawer::~NewParticleDrawer ()
{
}


void NewParticleDrawer::setup (const vector<Particle> &particles)
{
  geom::BufferLayout particleLayout;
  particleLayout.append (geom::Attrib::POSITION, 3, sizeof( Particle ), offsetof( Particle, mPosition ) );
  particleLayout.append (geom::Attrib::COLOR,    4, sizeof( Particle ), offsetof( Particle, mColor ) );
  particleLayout.append (geom::Attrib::CUSTOM_0, 1, sizeof( Particle ), offsetof( Particle, mCurrentSize ) );
  
  m_particleVbo = gl::Vbo::create (GL_ARRAY_BUFFER, particles, GL_STREAM_DRAW );
  auto mesh = gl::VboMesh::create ((uint32_t) particles.size (), GL_POINTS, { { particleLayout, m_particleVbo } } );
  
  gl::GlslProgRef shader = gl::GlslProg::create(app::loadAsset("pointsprite_es3.vert"), app::loadAsset("pointsprite_es3.frag"));
  m_particleBatch = gl::Batch::create( mesh, shader, { { geom::Attrib::CUSTOM_0, "vfSize" } }  );
}

void NewParticleDrawer::beforeDraw ()
{
}

void NewParticleDrawer::drawParticles (const vector<Particle> &particles, const size_t nofParticles, const vec2 &textureSize)
{
  // Copy particle data onto the GPU.
  // Map the GPU memory and write over it.
  void *gpuMem = m_particleVbo->mapReplace();
  memcpy (gpuMem, particles.data (), nofParticles * sizeof(Particle) );
  m_particleVbo->unmap();
  
  m_particleBatch->draw();
  
  /*
  
  gl::ScopedGlslProg GlslProgScope (m_shader);
  
  for (size_t i=0UL; i<nofParticles; i++)
  {
    const Particle *p = &particles[i];
    gl::color (p->mColor);
    myDrawBillboard (p->mPosition, vec2 (p->mCurrentSize, p->mCurrentSize), 0, vec3(1,0,0), vec3(0,1,0));
  }
   */
}

void NewParticleDrawer::afterDraw ()
{
}


void NewParticleDrawer::myDrawBillboard( const vec3 &pos, const vec2 &scale, float rotationRadians, const vec3 &bbRight, const vec3 &bbUp, const Rectf &texCoords )
{
  /*
  auto ctx = gl::context();
  
  GLfloat data[12+8]; // both verts and texCoords
  vec3 *verts = (vec3*)data;
  float *texCoordsOut = data + 12;
  
  float sinA = math<float>::sin( rotationRadians );
  float cosA = math<float>::cos( rotationRadians );
  
  verts[0] = pos + bbRight * ( -0.5f * scale.x * cosA - 0.5f * sinA * scale.y ) + bbUp * ( -0.5f * scale.x * sinA + 0.5f * cosA * scale.y );
  texCoordsOut[0*2+0] = texCoords.getX1(); texCoordsOut[0*2+1] = texCoords.getY1();
  verts[1] = pos + bbRight * ( -0.5f * scale.x * cosA - -0.5f * sinA * scale.y ) + bbUp * ( -0.5f * scale.x * sinA + -0.5f * cosA * scale.y );
  texCoordsOut[1*2+0] = texCoords.getX1(); texCoordsOut[1*2+1] = texCoords.getY2();
  verts[2] = pos + bbRight * ( 0.5f * scale.x * cosA - 0.5f * sinA * scale.y ) + bbUp * ( 0.5f * scale.x * sinA + 0.5f * cosA * scale.y );
  texCoordsOut[2*2+0] = texCoords.getX2(); texCoordsOut[2*2+1] = texCoords.getY1();
  verts[3] = pos + bbRight * ( 0.5f * scale.x * cosA - -0.5f * sinA * scale.y ) + bbUp * ( 0.5f * scale.x * sinA + -0.5f * cosA * scale.y );
  texCoordsOut[3*2+0] = texCoords.getX2(); texCoordsOut[3*2+1] = texCoords.getY2();
  
  ctx->pushVao();
  ctx->getDefaultVao()->replacementBindBegin();
  gl::VboRef defaultVbo = ctx->getDefaultArrayVbo( sizeof(float)*20 );
  gl::ScopedBuffer bufferBindScp( defaultVbo );
  defaultVbo->bufferSubData( 0, sizeof(float)*20, data );
  
  int posLoc = m_shader->getAttribSemanticLocation( geom::Attrib::POSITION );
  gl::enableVertexAttribArray( posLoc );
  gl::vertexAttribPointer( posLoc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );
  int texLoc = m_shader->getAttribSemanticLocation( geom::Attrib::TEX_COORD_0 );
  gl::enableVertexAttribArray( texLoc );
  gl::vertexAttribPointer( texLoc, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float)*12) );
  
  ctx->getDefaultVao()->replacementBindEnd();
  ctx->setDefaultShaderVars();
  ctx->drawArrays( GL_TRIANGLE_STRIP, 0, 4 );
  ctx->popVao();
  */
}

