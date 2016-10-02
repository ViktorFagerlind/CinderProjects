#include "VfBaseApp.h"

#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Vbo.h"
#include "cinder/Rand.h"
#include "cinder/gl/Vbo.h"
#include "cinder/ObjLoader.h"
#include "cinder/Timeline.h"

#include "Seed.h"
#include "Grass.h"
#include "Tree.h"
#include "BloomEffect.h"
#include "ShaderHelper.h"
#include "MiscMath.h"
#include "BumpMaterial.h"
#include "ParticleSystemManager.h"


using namespace ci;
using namespace ci::app;
using namespace std;

class VegetationPlanetApp : public VfBaseApp 
{
public:
	void setup();
  void keyDown (KeyEvent event);
  void mouseDown (MouseEvent event);
	void update ();
	void draw ();

	void spawnPatch (const vec3& spawnDirection);
	void clearPatches ();

private:
  std::vector<shared_ptr<Grass>> m_grassPatches;

  std::vector<shared_ptr<Tree>>  m_trees;

  shared_ptr<BloomEffect>        m_bloomEffect;

  float                          m_planetRadius;

  shared_ptr<BumpMaterial>       m_material;

  gl::VboMesh                    m_planetMesh;

  vec3                          m_sceneRotation;

  shared_ptr<Seed>               m_seed;
};

void VegetationPlanetApp::setup()
{
  VfBaseApp::setup (80.0f, 1.0f);

  float white[] = {1.0f, 1.0f, 1.0f , 0.0f};

  float p1[] = {-1000, 2000, 2000};

  glLightfv(GL_LIGHT0, GL_SPECULAR, white);
  glLightfv(GL_LIGHT0, GL_DIFFUSE,  white);
  glLightfv(GL_LIGHT0, GL_AMBIENT,  white);
	glLightfv(GL_LIGHT0, GL_POSITION, p1);
	glEnable (GL_LIGHT0);

  // Load mesh
  ObjLoader    loader (loadFile ("../Media/Meshes/Sphere.obj"));
  TriMesh      bodyTriMesh;
  loader.load (&bodyTriMesh);
	m_planetMesh = gl::VboMesh (bodyTriMesh);

  // Load planet material
	gl::Texture earthColor	= gl::Texture (loadImage (loadFile ("../Media/Images/myPlanetDiffuse.jpg")));
	gl::Texture earthNormal	= gl::Texture (loadImage (loadFile ("../Media/Images/myPlanetNormal.jpg")));
	earthColor.setWrap (GL_REPEAT, GL_REPEAT);
	earthNormal.setWrap (GL_REPEAT, GL_REPEAT);

  gl::GlslProg bumpShader = ShaderHelper::loadShader("../Media/Shaders/BumpMap_Vertex.glsl", 
                                                     "../Media/Shaders/BumpMap_Pixel.glsl");

  m_material.reset (new BumpMaterial (earthColor, // const gl::Texture&  diffuseTexture,
                                      earthNormal, // const gl::Texture&  normalTexture,
                                      bumpShader, // const gl::GlslProg& shader,
                                      bodyTriMesh,       // const TriMesh&      mesh, 
                                      ColorAf (0.06f, 0.05f,  0.05f, 1.f), // const ColorAf&      matAmbient,
                                      ColorAf (1.0f, 1.0f, 1.0f, 1.0f),    // const ColorAf&      matDiffuse,
                                      ColorAf (1.0f, 1.0f, 1.0f, 1.0f),    // const ColorAf&      matSpecular,
                                      40.0f)); // const float         matShininess);

  /*m_material.reset (new PhongMaterial (ShaderHelper::loadShader ("../Media/Shaders/phong_vert.glsl", 
                                                                 "../Media/Shaders/phong_frag.glsl"), 
                                       ColorAf (0.06f, 0.05f,  0.05f, 1.f), 
                                       ColorAf (0.12f, 0.10f,  0.10f, 1.f), 
                                       ColorAf (0.12f, 0.10f,  0.10f, 1.f), 
                                       1.f));*/

  // Misc things
  m_bloomEffect.reset (new BloomEffect (getWindowWidth()/4, getWindowHeight()/4, getWindowWidth(), getWindowHeight()));

  m_planetRadius = 20.f;

  m_sceneRotation = vec3 (0,0,0);

  m_seed.reset (new Seed());

//  hideCursor ();
}

void VegetationPlanetApp::mouseDown (MouseEvent event)
{
  if (event.isLeft ())
    spawnPatch (MiscMath::getRandomDirection ().normalized ());
  else if (event.isRight ())
    clearPatches ();
}

void VegetationPlanetApp::keyDown (KeyEvent event)
{
  VfBaseApp::keyDown (event);

  char c = event.getChar ();

  if (c == 's')
    m_seed->spawn ();
}


void VegetationPlanetApp::update()
{
  static uint32_t growSlow = 0;

  if (m_paused)
    return;

  m_sceneRotation.x += 0.2f;
  m_sceneRotation.y += 0.2f;
  m_sceneRotation.z += 0.7f;

  if (m_seed->m_position.value().lengthSquared () < m_planetRadius*m_planetRadius)
  {
    spawnPatch (m_seed->m_position.value().normalized ());

    m_seed->spawn ();
  }

  if ((growSlow % 3) == 0)
  {
    for (uint32_t i=0; i<m_grassPatches.size (); i++)
      m_grassPatches[i]->update ();

    for (uint32_t i=0; i<m_trees.size (); i++)
      m_trees[i]->update ();

    Tree::incrementAnimationCounter ();
  }

  growSlow++;

  m_seed->update ();

  ParticleSystemManager::getSingleton().update ();
}

void VegetationPlanetApp::draw()
{
  // Draw to frame buffer from now on
  m_frameBuffer->bindFramebuffer();

	// clear the window with black
	gl::clear (Color (0, 0, 0)); 

  gl::enableDepthRead ();
  gl::enableDepthWrite ();

  if (m_wireFrameMode)
    gl::enableWireframe ();

  // Setup camera
  mCamera->setMatrices ();

  // Rotate scene
  gl::rotate (m_sceneRotation);


  // Draw planet
  m_material->bind ();
  gl::pushModelView ();

  gl::enable  (GL_RESCALE_NORMAL);
  gl::scale   (m_planetRadius, m_planetRadius, m_planetRadius);

  gl::draw (m_planetMesh);

  gl::disable  (GL_RESCALE_NORMAL);

  gl::popModelView ();
  m_material->unbind ();

  // Draw grass
  for (uint32_t i=0; i<m_grassPatches.size (); i++)
    m_grassPatches[i]->draw ();

  // Draw trees
  for (uint32_t i=0; i<m_trees.size (); i++)
    m_trees[i]->draw ();

  // Draw seed
  m_seed->draw (m_planetMesh);

  // Draw particle system
	gl::disableDepthWrite ();
	gl::enableAdditiveBlending();

  ParticleSystemManager::getSingleton().draw ();

	gl::disableAlphaBlending();
	gl::enableDepthWrite ();

  m_frameBuffer->unbindFramebuffer();

  // Draw blooming effect
  gl::Fbo& bloomedFbo = m_bloomEffect->render (*m_frameBuffer.get());

  m_frameBuffer->bindFramebuffer ();
  gl::setViewport (getWindowBounds ());
  gl::setMatricesWindow (getWindowSize (), false);
	gl::enableAdditiveBlending();
  gl::color (1, 1, 1, 0.5f);
	gl::draw (bloomedFbo.getTexture(), bloomedFbo.getTexture().getBounds(), getWindowBounds());
	gl::disableAlphaBlending();
  m_frameBuffer->unbindFramebuffer ();

  gl::disableWireframe ();


  VfBaseApp::drawToScreen ();
}

void VegetationPlanetApp::spawnPatch (const vec3& spawnDirection)
{
  for (uint32_t i=0; i<15; i++)
  {
    shared_ptr<Grass> grass(new Grass (m_planetRadius,     // Size
                                        spawnDirection));   // Direction
    m_grassPatches.push_back (grass);
  }
    
  for (uint32_t i=0; i<10; i++)
  {
    vec3 direction = (spawnDirection*3.0f + MiscMath::getRandomDirection ()).normalized ();
    shared_ptr<Tree> tree(new Tree (direction*m_planetRadius*0.9f,                                           // Position
                                    (direction + MiscMath::getRandomDirection ()*Rand::randFloat (.3f,.6f)).normalized ())); // Direction
    m_trees.push_back (tree);
  }
}

void VegetationPlanetApp::clearPatches ()
{
  m_grassPatches.clear ();
  m_trees.clear ();
}


CINDER_APP_BASIC( VegetationPlanetApp, RendererGl )
