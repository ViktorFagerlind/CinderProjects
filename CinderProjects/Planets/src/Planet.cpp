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
                const Vec3f&          color, 
                const PhysicsObject&  gravityCenter, 
                const string&         textureFile)
: PhysicsObject (4.0f/3.0f * (float)M_PI * radius * radius * radius * density,
                 Vec3f(0, 0, 0),
                 radius),
  mGravityCenter (gravityCenter)
{
  float trackRadius = a * (1.0f - e);

  mL      = mMass * initialVelocity * trackRadius;

  mA      = a;
  mE      = e;
  mTheta		= 0.0f;
  mAxisAngle	= 0.0f;

  mColor = color;

  mTexture = new gl::Texture (loadImage (loadFile (textureFile)));

  GameWorld::getGravityField ()->addGravityObject (this);

  ObjLoader loader (loadFile ("../Media/Meshes/Sphere.obj"));
  loader.load (&mMesh);


	mEarthColor	  = gl::Texture (loadImage (loadFile ("../Media/Images/earthDiffuse.jpg")));
	mEarthNormal	= gl::Texture (loadImage (loadFile ("../Media/Images/earthMyNormals.jpg")));
//	mEarthColor	  = gl::Texture (loadImage (loadFile ("../Media/Images/brick_color_map.jpg")));
//	mEarthNormal	= gl::Texture (loadImage (loadFile ("../Media/Images/brick_normal_map.jpg")));
//	mEarthColor	  = gl::Texture (loadImage (loadFile ("../Media/Images/bump_own_diffuse.png")));
//	mEarthNormal	= gl::Texture (loadImage (loadFile ("../Media/Images/bump_own_normals.png")));
//	mEarthNormal	= gl::Texture (loadImage (loadFile ("../Media/Images/own_normals_none.png")));
	mEarthColor.setWrap( GL_REPEAT, GL_REPEAT );
	mEarthNormal.setWrap( GL_REPEAT, GL_REPEAT );
	try 
  {
		mPlanetShader = gl::GlslProg (loadFile ("../Media/Shaders/BumpMap_Vertex.glsl"), loadFile ("../Media/Shaders/BumpMap_Pixel.glsl"));
		//mPlanetShader = gl::GlslProg (loadFile ("../Media/Shaders/BumpMap_Vertex.glsl"), loadFile ("../Media/Shaders/test1_frag.glsl"));
	}
	catch (gl::GlslProgCompileExc &exc)
  {
		std::cout << "Shader compile error: " << std::endl;
		std::cout << exc.what();
	}
	catch (...) 
  {
		std::cout << "Unable to load shader" << std::endl;
	}

  std::vector<Vec3f>&	          meshNormals   = mMesh.getNormals ();
	const std::vector<Vec3f>&	    meshVertices  = mMesh.getVertices ();
  const std::vector<Vec2f>&			meshTexCoords = mMesh.getTexCoords();
	const std::vector<uint32_t>&  meshIndices   = mMesh.getIndices();

  const unsigned int nofTriangles = mMesh.getNumTriangles ();
  const unsigned int nofVertices  = meshVertices.size ();

  vector<Vec3f> addedTangents;
  vector<unsigned int> nofAdditions;
  addedTangents.resize (nofVertices); // One tangent (3 floats) for each vertex
  nofAdditions.resize (nofVertices);

  // Go through the triangles and sum the tangents for the different faces
  for (int t=0; t<nofTriangles; t++)
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

    Vec3f tangent;
    MiscMath::ComputeTangent (p1, p2, p3, uv1, uv2, uv3, tangent);


    // Write the same tangent to all points (!?!?!?!?!?)
    addedTangents[i1] += tangent;
    addedTangents[i2] += tangent;
    addedTangents[i3] += tangent;

    nofAdditions[i1]++;
    nofAdditions[i2]++;
    nofAdditions[i3]++;
  }

  // Calculate the mean tangent and to float vector format
  mTangentBuffer.resize (nofVertices * 3); // One tangent (3 floats) for each vertex
  for (int i=0; i<nofVertices; i++)
  {
    mTangentBuffer[3*i]   = addedTangents[i].x / (float)nofAdditions[i];
    mTangentBuffer[3*i+1] = addedTangents[i].y / (float)nofAdditions[i];
    mTangentBuffer[3*i+2] = addedTangents[i].z / (float)nofAdditions[i];
  }

  /*
  for (int i=200; i<400; i++)
  {
    Vec3f tangent = Vec3f (1,1,1);

    mTangentBuffer[3*i]   = tangent.x;
    mTangentBuffer[3*i+1] = tangent.y;
    mTangentBuffer[3*i+2] = tangent.z;
  }
  */

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
  mPosition = mGravityCenter.getPosition () + Vec3f(r * cos(mTheta), r * sin(mTheta), 0.0f);

  mAxisAngle += 0.5f;
}

//----------------------------------------------------------------------------------------------------------------------

void Planet::draw()
{
  // Matrix transforms
  gl::pushModelView();
  gl::translate(mPosition);
  gl::rotate(Vec3f(45.0f,  0, 20.0f));
  gl::rotate(Vec3f(0.0f,  mAxisAngle, 0));

  // Setup material for planets
  float mat_ambient[]		 = {0.0f, 0.0f, 0.0f, 1.0f};
  float mat_diffuse[]		 = {1.0f, 1.0f, 1.0f, 1.0f};
  float mat_specular[]	 = {1.0f, 1.0f, 1.0f, 1.0f};
  float mat_shininess	   = 40.0f;
	glMaterialfv (GL_FRONT, GL_AMBIENT,	   mat_ambient);
  glMaterialfv (GL_FRONT, GL_DIFFUSE,	   mat_diffuse);
  glMaterialfv (GL_FRONT, GL_SPECULAR,   mat_specular);
  glMaterialfv (GL_FRONT, GL_SHININESS, &mat_shininess);

	glDisable (GL_BLEND);
  glEnable (GL_TEXTURE_2D);
  
	mPlanetShader.bind();
	mPlanetShader.uniform( "colorMap", 0 );
	mPlanetShader.uniform( "normalMap", 1 );
	mPlanetShader.uniform( "invRadius", 1.0f/1000.0f ); // Inverse of the sun ligting radius
  
	mEarthColor.bind (0);
	mEarthNormal.bind (1);

	GLint tangentUniformLoc = mPlanetShader.getAttribLocation ("vTangent");
  glEnableVertexAttribArrayARB (tangentUniformLoc);
  glVertexAttribPointerARB (tangentUniformLoc, 3, GL_FLOAT, GL_FALSE, 0, mTangentBuffer.data());

  glEnable (GL_RESCALE_NORMAL);
  gl::scale (mRadius, mRadius, mRadius);

  gl::draw (mMesh);
  glDisable (GL_RESCALE_NORMAL);

  glDisableVertexAttribArrayARB (tangentUniformLoc);

	mEarthColor.unbind (0);
	mEarthNormal.unbind (1);
	mPlanetShader.unbind();

  const std::vector<Vec3f>&	    meshNormals   = mMesh.getNormals ();
	const std::vector<uint32_t>&	meshIndices   = mMesh.getIndices();
	const std::vector<Vec3f>&	    meshVertices  = mMesh.getVertices ();
  
#if 0
  // Draw normals and tangents
  for (int i=0; i<mMesh.getNumTriangles (); i++)
  {
    Vec3f tangent;

    const int i1 = meshIndices[3*i];
    const int i2 = meshIndices[3*i+1];
    const int i3 = meshIndices[3*i+2];

    glColor3f (1, 0, 0);
    gl::drawVector (meshVertices[i1], meshVertices[i1]+meshNormals[i1]);

    glColor3f (0, 1, 0);
    tangent = Vec3f (mTangentBuffer[3*i1], mTangentBuffer[3*i1+1], mTangentBuffer[3*i1+2]);
    gl::drawVector (meshVertices[i1], meshVertices[i1]+tangent);
  }
#endif

  gl::popModelView();
}



