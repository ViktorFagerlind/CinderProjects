#include "Tree.h"

#include "VegetationMaterials.h"
#include "TubeDrawer.h"
#include "ShaderHelper.h"
#include "MiscMath.h"
#include "BSpline.h"

#include "cinder/Matrix44.h"
#include "cinder/Rand.h"
#include "cinder/ObjLoader.h"
#include "cinder/app/AppBasic.h"
#include "cinder/Timeline.h"


Flower::Flower (const float size, const vec3& through, const vec3& left, const vec3& position)
: m_position (position),
  m_through  (through),
  m_left     (left)
{
  app::App *app = app::App::get ();

  vec3 up   = through.cross (left);
  m_orientation = Matrix44f::createRotationOnb (left, through, up);

  app->timeline ().apply (&m_size, size/4.f, size, 0.2f, EaseNone());
}

//----------------------------------------------------------------------------------------------------------------------

gl::VboMesh& Flower::getVboMesh ()
{
  static gl::VboMesh vboMeshInstance;
  static bool        isInstantiated = false;

  if (isInstantiated)
    return vboMeshInstance;

  //-- Create leaf VBO ------------------------------------------
 	std::vector<uint32_t> vboIndices;
	std::vector<vec3>    vboVertices;
	std::vector<vec3>    vboNormals;

  // Create indicies
  for (uint32_t i=0; i<11; i++)
    vboIndices.push_back (i);

  // Create verticies and normals
  vboNormals.push_back  (vec3 ( .1f, -0.2f, 1));
  vboVertices.push_back (vec3 ( 1.f,  0.f, -2.f));
  vboNormals.push_back  (vec3 (-.1f, -0.2f, 1));
  vboVertices.push_back (vec3 (-1.f,  0.f, -2.f));

  vboNormals.push_back  (vec3 ( .1f, -0.1f, 1));
  vboVertices.push_back (vec3 ( 1.f,  4.f, -2.f));
  vboNormals.push_back  (vec3 (-.1f, -0.2f, 1));
  vboVertices.push_back (vec3 (-1.f,  4.f, -2.f));

  vboNormals.push_back  (vec3 ( .1f, -0.0f, 1));
  vboVertices.push_back (vec3 ( 6.f,  6.f,  -1.f));
  vboNormals.push_back  (vec3 (-.1f, -0.2f, 1));
  vboVertices.push_back (vec3 (-6.f,  6.f,  -1.f));

  vboNormals.push_back  (vec3 ( .1f, -0.1f, 1));
  vboVertices.push_back (vec3 ( 8.f, 10.f,   0.f));
  vboNormals.push_back  (vec3 (-.1f, -0.2f, 1));
  vboVertices.push_back (vec3 (-8.f, 10.f,   0.f));

  vboNormals.push_back  (vec3 ( .1f,  0.2f, 1));
  vboVertices.push_back (vec3 ( 6.f, 16.f,  -2.f));
  vboNormals.push_back  (vec3 (-.1f, -0.2f, 1));
  vboVertices.push_back (vec3 (-6.f, 16.f,  -2.f));
  
  vboNormals.push_back  (vec3 (0,  0.3f, 1));
  vboVertices.push_back (vec3 ( 0.f, 22.f,  -3.f));

  // Vbo settings
	gl::VboMesh::Layout layout;
	layout.setStaticIndices ();
	layout.setStaticPositions ();
	layout.setStaticNormals ();

  // Create actual Vbo
  vboMeshInstance = gl::VboMesh (vboVertices.size(), vboIndices.size(), layout, GL_TRIANGLE_STRIP);
	vboMeshInstance.bufferIndices (vboIndices);
	vboMeshInstance.bufferPositions (vboVertices);
  vboMeshInstance.bufferNormals (vboNormals);
	vboMeshInstance.unbindBuffers ();	// Clean up

  isInstantiated = true;

  return vboMeshInstance;
}

//----------------------------------------------------------------------------------------------------------------------

void Flower::draw ()
{
  gl::pushModelView ();

  gl::translate (m_position);

  gl::multModelView (m_orientation);

  float scale = m_size.value ();
  gl::scale (scale, scale, scale);

  gl::enable  (GL_RESCALE_NORMAL);

  // lean down leaf
  Matrix44f layRotation = Matrix44f::createRotation (vec3 (1,0,0), -70.f * (float)M_PI / 180.f);

  // rotate leaf around center
  const uint32_t nofFlowerLeaves = 5;
  Matrix44f manyRotation = Matrix44f::createRotation (vec3 (0,1,0), 2.f * (float)M_PI / (float) nofFlowerLeaves);
  for (uint32_t i=0; i<nofFlowerLeaves; i++)
  {
    gl::pushModelView ();
    gl::multModelView (layRotation);

    gl::draw (getVboMesh ());
    gl::popModelView ();

    gl::multModelView (manyRotation);
  }

  gl::disable (GL_RESCALE_NORMAL);

  gl::popModelView ();

//  gl::drawVector (m_position, m_position + m_through);
}

//----------------------------------------------------------------------------------------------------------------------

Leaf::Leaf (const float size, const vec3& through, const vec3& left, const vec3& position)
: m_position (position)
{
  vec3 up = through.cross (left);

  m_orientation = Matrix44f::createRotationOnb (left, through, up);

  app::App *app = app::App::get ();

  app->timeline ().apply (&m_size, size/4.f, size, 1.f, EaseNone());
}

//----------------------------------------------------------------------------------------------------------------------

gl::VboMesh& Leaf::getVboMesh ()
{
  static gl::VboMesh vboMeshInstance;
  static bool        isInstantiated = false;

  if (isInstantiated)
    return vboMeshInstance;

  //-- Create leaf VBO ------------------------------------------
 	std::vector<uint32_t> vboIndices;
	std::vector<vec3>    vboVertices;
	std::vector<vec3>    vboNormals;

  // Create indicies
  for (uint32_t i=0; i<11; i++)
    vboIndices.push_back (i);

  // Create verticies and normals
  vboNormals.push_back  (vec3 ( .1f, -0.2f, 1));
  vboVertices.push_back (vec3 ( 1.f,  0.f, -2.f));
  vboNormals.push_back  (vec3 (-.1f, -0.2f, 1));
  vboVertices.push_back (vec3 (-1.f,  0.f, -2.f));

  vboNormals.push_back  (vec3 ( .1f, -0.1f, 1));
  vboVertices.push_back (vec3 ( 1.f,  4.f, -2.f));
  vboNormals.push_back  (vec3 (-.1f, -0.2f, 1));
  vboVertices.push_back (vec3 (-1.f,  4.f, -2.f));

  vboNormals.push_back  (vec3 ( .1f, -0.0f, 1));
  vboVertices.push_back (vec3 ( 6.f,  6.f,  -1.f));
  vboNormals.push_back  (vec3 (-.1f, -0.2f, 1));
  vboVertices.push_back (vec3 (-6.f,  6.f,  -1.f));

  vboNormals.push_back  (vec3 ( .1f, -0.1f, 1));
  vboVertices.push_back (vec3 ( 8.f, 10.f,   0.f));
  vboNormals.push_back  (vec3 (-.1f, -0.2f, 1));
  vboVertices.push_back (vec3 (-8.f, 10.f,   0.f));

  vboNormals.push_back  (vec3 ( .1f,  0.2f, 1));
  vboVertices.push_back (vec3 ( 6.f, 16.f,  -2.f));
  vboNormals.push_back  (vec3 (-.1f, -0.2f, 1));
  vboVertices.push_back (vec3 (-6.f, 16.f,  -2.f));
  
  vboNormals.push_back  (vec3 (0,  0.3f, 1));
  vboVertices.push_back (vec3 ( 0.f, 22.f,  -3.f));

  // Vbo settings
	gl::VboMesh::Layout layout;
	layout.setStaticIndices ();
	layout.setStaticPositions ();
	layout.setStaticNormals ();

  // Create actual Vbo
  vboMeshInstance = gl::VboMesh (vboVertices.size(), vboIndices.size(), layout, GL_TRIANGLE_STRIP);
	vboMeshInstance.bufferIndices (vboIndices);
	vboMeshInstance.bufferPositions (vboVertices);
  vboMeshInstance.bufferNormals (vboNormals);
	vboMeshInstance.unbindBuffers ();	// Clean up

  isInstantiated = true;

  return vboMeshInstance;
}

//----------------------------------------------------------------------------------------------------------------------

void Leaf::draw ()
{
  gl::pushModelView ();

  gl::translate (m_position);

  gl::multModelView (m_orientation);

  float scale = m_size.value ();
  gl::scale (scale, scale, scale);

  gl::enable  (GL_RESCALE_NORMAL);

  gl::draw (getVboMesh ());

  gl::disable (GL_RESCALE_NORMAL);

  gl::popModelView ();

}


//----------------------------------------------------------------------------------------------------------------------

Branch::Branch (const float               radius, 
                const vec3&              position, 
                const vec3&              previusNormal, 
                const vec3&              normal, 
                const uint32_t            fullLife,
                const float               jointLength)
: m_radius        (radius),
  m_position      (position),
  m_normal        (normal),
  m_life          (0),
  m_fullLife      (fullLife),
  m_jointLength   (jointLength),
  m_jointLife     (0.f),
  m_currentJoint  (0)
{
  // Create initial joints to make sure the branch starts at position
  m_joints.push_back (BranchJoint (m_position - previusNormal * jointLength, m_normal, m_jointLength));
  m_joints.push_back (BranchJoint (m_position                         , m_normal, m_jointLength));
  m_joints.push_back (BranchJoint (m_position + previusNormal * jointLength, m_normal, m_jointLength));
  m_joints.push_back (BranchJoint (m_joints.back().getEndPosition (), (m_normal + MiscMath::getRandomDirection()*0.3f).normalized (), m_jointLength));
}

//----------------------------------------------------------------------------------------------------------------------

void Branch::update ()
{
  updateSelf ();

  for (std::list<Branch>::iterator it=m_subBranches.begin (); it != m_subBranches.end (); it++)
    it->update ();
}

//----------------------------------------------------------------------------------------------------------------------

void Branch::updateSelf ()
{
  if (m_life == m_fullLife)
    return;

  if ((Tree::getAnimationCounter () % 2) != 0) // Speed up by only animate at half the framerate
    return;

  vec3 newPoint = VfBSpline::calc3D (m_joints[m_currentJoint  ].m_position,
                                      m_joints[m_currentJoint+1].m_position,
                                      m_joints[m_currentJoint+2].m_position,
                                      m_joints[m_currentJoint+3].m_position,
                                      m_jointLife);
  m_drawPoints.push_back (newPoint);

  m_jointLife += .3f;

  float growPercent = ((float)m_life + m_jointLife) / (float)m_fullLife;

  const vec3& currentNormal = m_joints[m_currentJoint+1].m_normal; // Current dírection of the branch

  if (growPercent > 0.4f && Rand::randFloat(0.f, growPercent) > 0.1f) // Create leaf?
  {
    vec3 through = (currentNormal + MiscMath::getRandomDirection()*0.5f).normalized ();
    vec3 left    = (through.cross(-newPoint) + MiscMath::getRandomDirection()*0.5f).normalized ();

    m_leaves.push_back (Leaf (Rand::randFloat (0.02f, 0.04f),   // Size
                              through,                          // Through
                              left,                             // Left
                              newPoint));                       // Position
  }

  if (growPercent > 0.7f && Rand::randFloat(0.f, growPercent) > 0.5f) // Create flower?
  {
    vec3 through = (currentNormal + MiscMath::getRandomDirection()*1.0f).normalized ();
    vec3 left    = (through.cross(-newPoint) + MiscMath::getRandomDirection()*0.5f).normalized ();

    m_flowers.push_back (Flower (Rand::randFloat (0.01f, 0.02f),   // Size
                                 through,                          // Normal
                                 left,                             // Left
                                 newPoint));                       // Position

  }

  if (m_jointLife >= 1.f) // New joint is needed
  {
    m_joints.push_back (BranchJoint (m_joints.back().getEndPosition (),                              // Position
                                     (m_normal + MiscMath::getRandomDirection()*0.3f).normalized (), // Normal
                                     m_jointLength));                                                // Joint length
    m_currentJoint++;

    if (Rand::randFloat(0.f, growPercent) < 0.25f && m_life < m_fullLife-2 && m_life > 0) // New branch?
    {
      float radius       = VfBSpline::calc1D (2.f*m_radius, m_radius, 0.f, -m_radius, growPercent);

      vec3 rotationAxis = currentNormal.cross (MiscMath::getRandomDirection());
      vec3 branchNormal = currentNormal;
      branchNormal.rotate (rotationAxis, Rand::randFloat (toRadians (30.f), toRadians (90.f)));

      m_subBranches.push_back (Branch (radius,                      // radius
                                       newPoint,                    // position
                                       m_joints[m_currentJoint+1].m_normal,
                                       branchNormal,                // normal
                                       m_fullLife-1,                // fullLife
                                       m_jointLength));             // jointLength);
    }

    m_jointLife -= 1.f; // Start over on new joint

    m_life++;
  }
}

//----------------------------------------------------------------------------------------------------------------------

void Branch::drawSelf (gl::GlslProg& shader)
{
  uint32_t nofPoints = m_drawPoints.size ();

  if (nofPoints < 3)
    return;

  // double the size if not enough
  if (nofPoints > m_normals.size ())
  {
    m_normals.resize (nofPoints * 2);
    m_radie.resize   (nofPoints * 2);
  }

  m_radie[0]   = VfBSpline::calc1D (2.f*m_radius, m_radius, 0.f, -m_radius, 0.f);
  m_normals[0] = (m_drawPoints[1] - m_drawPoints[0]).normalized ();

  for (uint32_t i=0; i<nofPoints - 2; i++)
  {
    vec3 currentToNext  = m_drawPoints[i+1] - m_drawPoints[i];
    vec3 nextToNextNext = m_drawPoints[i+2] - m_drawPoints[i+1];

    m_radie[i+1]   = VfBSpline::calc1D (2.f*m_radius, m_radius, 0.f, -m_radius, (float)i/(float)(nofPoints-3));
    m_normals[i+1] = (currentToNext + nextToNextNext).normalized ();
  }

  // Choose a general direction for the "up" vector so that it is perpendicular to the 
  // general layout of the entire line, in that way the face normals are less likely 
  // to be aligned to the direction (which is not good when projecting on that plane)
  vec3 upDirection = vec3 (0,0,1).cross (m_joints.back().m_position - m_joints.front().m_position);

  TubeDrawer::draw (shader, 
                    upDirection,
                    false,
                    5,
                    m_drawPoints.data (),
                    m_normals.data (),
                    m_radie.data (),
                    nofPoints - 1);
}

//----------------------------------------------------------------------------------------------------------------------

void Branch::draw (gl::GlslProg& shader)
{
  drawSelf (shader);

  for (std::list<Branch>::iterator it=m_subBranches.begin (); it != m_subBranches.end (); it++)
    it->draw (shader);
}

//----------------------------------------------------------------------------------------------------------------------

void Branch::drawLeaves ()
{
  for (std::list<Leaf>::iterator it=m_leaves.begin (); it != m_leaves.end (); it++)
    it->draw ();

  for (std::list<Branch>::iterator it=m_subBranches.begin (); it != m_subBranches.end (); it++)
    it->drawLeaves ();
}


//----------------------------------------------------------------------------------------------------------------------

void Branch::drawFlowers ()
{
  for (std::list<Flower>::iterator it=m_flowers.begin (); it != m_flowers.end (); it++)
    it->draw ();

  for (std::list<Branch>::iterator it=m_subBranches.begin (); it != m_subBranches.end (); it++)
    it->drawFlowers ();
}

//----------------------------------------------------------------------------------------------------------------------

uint32_t Tree::m_animationCounter = 0;

//----------------------------------------------------------------------------------------------------------------------

Tree::Tree (const vec3&        position, 
            const vec3&        direction)
: m_branchMaterial (VegetationMaterials::getSingleton ().getBranchMaterial  ()),
  m_leafMaterial   (VegetationMaterials::getSingleton ().getLeafMaterial    ()),
  m_flowerMaterial (VegetationMaterials::getSingleton ().getFlowerMaterial  ()),
  m_branch (0.6f,        // radius
            position,   // position
            direction,  // previous normal
            direction,  // normal
            6,          // life
            2.f)        // jointLength
{
}

//----------------------------------------------------------------------------------------------------------------------

void Tree::update ()
{
  m_branch.update ();
}
	
//----------------------------------------------------------------------------------------------------------------------

void Tree::draw ()
{
  m_branchMaterial->bind ();
  m_branch.draw (m_branchMaterial->getShader ());
  m_branchMaterial->unbind ();

  m_leafMaterial->bind ();
  m_branch.drawLeaves ();
  m_leafMaterial->unbind ();

  m_flowerMaterial->bind ();
  m_branch.drawFlowers ();
  m_flowerMaterial->unbind ();
}

//----------------------------------------------------------------------------------------------------------------------

