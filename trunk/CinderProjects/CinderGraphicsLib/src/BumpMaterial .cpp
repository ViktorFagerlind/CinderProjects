#include "BumpMaterial.h"

#include "MiscMath.h"

const int colorMapIndex  = 0;
const int normalMapIndex = 1;

BumpMaterial::BumpMaterial (      gl::Texture   diffuseTexture,
                                  gl::Texture   normalTexture,
                                  gl::GlslProg  shader,
                            const TriMesh&      mesh, 
                            const ColorAf&      matAmbient,
                            const ColorAf&      matDiffuse,
                            const ColorAf&      matSpecular,
                            const float         matShininess)
: PhongMaterial (shader, matAmbient, matDiffuse, matSpecular, matShininess)
{
  mDiffuseTexture = diffuseTexture;
	mNormalTexture  = normalTexture;
  mShader         = shader;

  calculateTangents (mesh);
}

void BumpMaterial::calculateTangents (const TriMesh& mesh)
{
	const std::vector<Vec3f>&	    meshVertices  = mesh.getVertices ();
  const std::vector<Vec2f>&			meshTexCoords = mesh.getTexCoords();
	const std::vector<uint32_t>&  meshIndices   = mesh.getIndices();

  const uint32_t nofVertices  = meshVertices.size ();

  vector<Vec3f>         addedTangents;
  addedTangents.resize (nofVertices); // One tangent (3 floats) for each vertex

  vector<uint32_t>  nofAdditions;
  nofAdditions.resize  (nofVertices);

  // Go through the triangles and sum the tangents for the different faces
  for (int t=0; t<mesh.getNumTriangles (); t++)
  {
    const int i1 = meshIndices[3*t];
    const int i2 = meshIndices[3*t+1];
    const int i3 = meshIndices[3*t+2];

    const Vec3f& p1  = meshVertices[i1];
    const Vec3f& p2  = meshVertices[i2];
    const Vec3f& p3  = meshVertices[i3];

    const Vec2f& uv1 = meshTexCoords[i1];
    const Vec2f& uv2 = meshTexCoords[i2];
    const Vec2f& uv3 = meshTexCoords[i3];

    Vec3f tmpTangent;
    MiscMath::ComputeTangent (p1, p2, p3, uv1, uv2, uv3, tmpTangent);


    // Write the same tangent to all points (!?!?!?!?!?)
    addedTangents[i1] += tmpTangent;
    addedTangents[i2] += tmpTangent;
    addedTangents[i3] += tmpTangent;

    nofAdditions[i1]++;
    nofAdditions[i2]++;
    nofAdditions[i3]++;
  }

  // Calculate the mean tangent and convert to float vector format
  mTangentBuffer.resize (nofVertices * 3); // One tangent (3 floats) for each vertex
  for (int i=0; i<nofVertices; i++)
  {
    mTangentBuffer[3*i]   = addedTangents[i].x / (float)nofAdditions[i];
    mTangentBuffer[3*i+1] = addedTangents[i].y / (float)nofAdditions[i];
    mTangentBuffer[3*i+2] = addedTangents[i].z / (float)nofAdditions[i];
  }
}

void BumpMaterial::bind ()
{
  PhongMaterial::bind ();

	mShader.uniform ("colorMap",  colorMapIndex);
	mShader.uniform ("normalMap", normalMapIndex);
	mShader.uniform ("invRadius", 1.0f/500.0f); // Inverse of the sun lighting radius
  
  mDiffuseTexture.bind (colorMapIndex);
	mNormalTexture.bind (normalMapIndex);

	mTangentUniformLoc = mShader.getAttribLocation ("vTangent");
  glEnableVertexAttribArrayARB (mTangentUniformLoc);
  glVertexAttribPointerARB (mTangentUniformLoc, 3, GL_FLOAT, GL_FALSE, 0, mTangentBuffer.data());
}

void BumpMaterial::unbind ()
{
  PhongMaterial::unbind ();

  glDisableVertexAttribArrayARB (mTangentUniformLoc);
  mDiffuseTexture.unbind (colorMapIndex);
	mNormalTexture.unbind (normalMapIndex);
}
