#include "Planet.h"

#include "cinder/ObjLoader.h"

#include "math.h"
#include "GravityField.h"
#include "GameWorld.h"
#include "MiscMath.h"

static const float density = 1000;

//----------------------------------------------------------------------------------------------------------------------

Planet::Planet (const float           a, 
                const float           e, 
                const float           radius, 
                const float           initialVelocity, 
                const PhysicsObject*  gravityCenter,
                      shared_ptr<BaseMaterial>   material,
                      gl::VboMesh     vbo)
: PhysicsObject  (4.0f/3.0f * (float)M_PI * radius * radius * radius * density,
                  vec3(0, 0, 0),
                  radius)
{
  float trackRadius = a * (1.0f - e);

  mL      = mMass * initialVelocity * trackRadius;

  mA      = a;
  mE      = e;
  mTheta		= 0.0f;
  mAxisAngle	= 0.0f;

  GameWorld::getGravityField ()->addGravityObject (this);

  mGravityCenter  = gravityCenter;
  mMaterial       = material;
  mVbo            = vbo;
}

//----------------------------------------------------------------------------------------------------------------------

/* eq. 1: r = a (1-e^2)/(1+e*cos(eta))
 * eq. 2: L = m*v*r  
 * eq. 3: deltaTheta = v/r
 */
void Planet::update()
{
  float r           = mA * (1.0f - mE*mE)/(1.0f+mE*cos(mTheta)); // (1)
  float v           = mL /(mMass*r);                       // (2)
  float deltaTheta  = v / r;                               // (3)

  mTheta    = mTheta + deltaTheta;
  mPosition = mGravityCenter->getPosition () + vec3(r * cos(mTheta), r * sin(mTheta), 0.0f);

  mAxisAngle += 0.5f;
}

//----------------------------------------------------------------------------------------------------------------------

void Planet::draw()
{
  // Matrix transforms
  gl::pushModelView();
  gl::translate(mPosition);
  gl::rotate(vec3(45.0f,       0.0f, 20.0f));
  gl::rotate(vec3( 0.0f, mAxisAngle,  0.0f));

  mMaterial->bind ();

  glEnable (GL_RESCALE_NORMAL);
  gl::scale (mRadius, mRadius, mRadius);

  gl::draw (mVbo);
  glDisable (GL_RESCALE_NORMAL);

  mMaterial->unbind ();

  gl::popModelView();
}

#if 0
  // Draw normals and tangents
  const std::vector<vec3>&	    meshNormals   = mMesh.getNormals ();
	const std::vector<uint32_t>&	meshIndices   = mMesh.getIndices();
	const std::vector<vec3>&	    meshVertices  = mMesh.getVertices ();
  
  for (int i=0; i<mMesh.getNumTriangles (); i++)
  {
    vec3 tangent;

    const int i1 = meshIndices[3*i];
    const int i2 = meshIndices[3*i+1];
    const int i3 = meshIndices[3*i+2];

    glColor3f (1, 0, 0);
    gl::drawVector (meshVertices[i1], meshVertices[i1]+meshNormals[i1]);

    glColor3f (0, 1, 0);
    tangent = vec3 (mTangentBuffer[3*i1], mTangentBuffer[3*i1+1], mTangentBuffer[3*i1+2]);
    gl::drawVector (meshVertices[i1], meshVertices[i1]+tangent);
  }
#endif




