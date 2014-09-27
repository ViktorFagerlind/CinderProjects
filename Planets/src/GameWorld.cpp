#include "GameWorld.h"

#include "cinder/gl/gl.h"
#include "cinder/gl/Vbo.h"
#include "cinder/ObjLoader.h"

#include "Planet.h"
#include "Meteor.h"

#include "AreaEmitter.h"
#include "CommonModifier.h"
#include "ColorModifier.h"
#include "PerlinModifier.h"
#include "PointEmitter.h"
#include "ShaderHelper.h"

#include <list>

GameWorld* GameWorld::theGameWorld = NULL;

uint32_t GameWorld::mScreenWidth  = 1600;
uint32_t GameWorld::mScreenHeight = 1000;


GameWorld* GameWorld::getSingleton ()
{
  if (theGameWorld == NULL)
  {
    theGameWorld = new GameWorld ();
    return theGameWorld;
  }
  else
    return theGameWorld;
}

void GameWorld::keyDown (KeyEvent event)
{
  mMovingCamera->keyDown (event);
}

void GameWorld::mouseMove (MouseEvent event)
{
  mMovingCamera->mouseMove (event);
}

void GameWorld::setup ()
{
  CommonModifier *commonModifier;
  ColorModifier  *colorModifier;

  mBloomEffect = new BloomEffect (mScreenWidth/8, mScreenHeight/8, mScreenWidth, mScreenHeight);

  mMovingCamera = new MovingCamera (500.0f);

  // Setup gravity field
  mGravityField = new GravityField (Vec3f (1000, 600, 200), Vec3i (51, 31, 13));

  // --- Initialise particle system ---------------------------------

  // --- The star dust -------------------
  ParticleSystem *particleSystem  = new ParticleSystem("../Media/Images/particle1.jpg");
  ParticleSystemManager::getSingleton().addParticleSystem (particleSystem);

  AreaEmitter *areaEmitter     = new AreaEmitter  (50000,          // maxNofParticles,
                                                   Vec3f (0, 0, 0), // position, 
                                                   10.0f,          // particlesPerFrame,
  						                                     650,             // width, 
							                                     600,             // height,
							                                     200,             // depth,
							                                     0.7f,               // minParticleSize,
							                                     0.7f,               // maxParticleSize,
                                                   Vec3f (0, 0, 0),     // baseVelocity
                                                   0.0f);    // randVelocity
  particleSystem->addEmitter (areaEmitter); 

  particleSystem->addModifier (mGravityField);

  commonModifier = new CommonModifier (1, 1.0f, 0.5f);
  particleSystem->addModifier (commonModifier);
    
  // --- The explosions -------------------
  mExposionSystem = new ParticleSystem ("../Media/Images/fire.png");
  commonModifier = new CommonModifier (3, 2.0f, 1.0f);
  colorModifier  = new ColorModifier (ColorAf(1, 1, 1, 0.5f), //startColor 
                                      ColorAf(1, 1, 1, 0.9f), //middleColor
                                      ColorAf(1, 1, 1, 0.0f), //endColor
                                      0.5f);//float middleTime)

  mExposionSystem->addModifier (commonModifier);
  mExposionSystem->addModifier (colorModifier);

  ParticleSystemManager::getSingleton().addParticleSystem (mExposionSystem);

  // --- Initialise planet system ---------------------------------

  // Load mesh
  ObjLoader loader (loadFile ("../Media/Meshes/Sphere.obj"));
  TriMesh      planetMesh;
  loader.load (&planetMesh);
	gl::VboMesh planetVbo = gl::VboMesh (planetMesh);

  mSun = new Sun (100.0f*1000.0f*1000.0f, Vec3f (100, 0, 0), 40, planetVbo);
  mObjects.push_back (mSun);

  shared_ptr<BumpMaterial> earthMaterial = getBumpMaterial (planetMesh, 
                                                            "../Media/Images/earthDiffuse.jpg",
                                                            "../Media/Images/earthMyNormals.jpg");
  shared_ptr<BumpMaterial> brickMaterial = getBumpMaterial (planetMesh, 
                                                            "../Media/Images/brick_color_map.jpg",
                                                            "../Media/Images/brick_normal_map.jpg");
  shared_ptr<BumpMaterial> ownMaterial = getBumpMaterial (planetMesh, 
                                                          "../Media/Images/bump_own_diffuse.png",
                                                          "../Media/Images/bump_own_normals.png");
  shared_ptr<BumpMaterial> greenMaterial = getBumpMaterial (planetMesh, 
                                                            "../Media/Images/green_diffuse.jpg",
                                                            "../Media/Images/green_normal.png");
  shared_ptr<BumpMaterial> moonMaterial = getBumpMaterial (planetMesh, 
                                                            "../Media/Images/moon.jpg",
                                                            "../Media/Images/bumps.jpg");
  shared_ptr<BumpMaterial> fireMaterial = getBumpMaterial (planetMesh, 
                                                            "../Media/Images/fire_planet_diffuse.jpg",
                                                            "../Media/Images/fire_planet_normal.jpg");

  vector<shared_ptr<BumpMaterial>> materials;
  materials.push_back (brickMaterial);
  materials.push_back (earthMaterial);
  materials.push_back (ownMaterial);
  materials.push_back (greenMaterial);
  materials.push_back (moonMaterial);
  materials.push_back (fireMaterial);

  for (int i=0; i<6; i++)
  {
    Planet *planet = new Planet(Rand::randFloat(100, 350),    // a
                                Rand::randFloat(0.5f, 0.99f),  // e
                                Rand::randFloat(15, 50),       // radius
                                Rand::randFloat(0.5f, 8),    // initial velocity
                                mSun, // center
                                materials[Rand::randInt(materials.size())],
                                planetVbo);
    
    mObjects.push_back (planet);

    for (int j = 0; j<2; j++)
    {
      Planet *moon = new Planet(planet->getRadius() + Rand::randFloat(10, 30),      // a
                                Rand::randFloat(0.0f, 0.5f),  // e
                                planet->getRadius() * 0.3f,   // radius
                                Rand::randFloat(1, 2),       // initial velocity
                                planet,              // center 
                                materials[Rand::randInt(materials.size())],
                                planetVbo);

     mObjects.push_back (moon);
    }
  }




  gl::GlslProg phongShader;
	try {
    //	int32_t maxGeomOutputVertices;
    //	glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT, &maxGeomOutputVertices);
		phongShader = gl::GlslProg (loadFile ("../Media/Shaders/phong_vert.glsl"), 
                                loadFile ("../Media/Shaders/phong_frag.glsl"));
	}	catch (gl::GlslProgCompileExc &exc) {
		std::cout << "Shader compile error: " << std::endl;
		std::cout << exc.what();
	}	catch (...) {
		std::cout << "Unable to load shader" << std::endl;
	}

  mMeteorMaterial.reset (new PhongMaterial (phongShader,                        // Shader
                                       ColorAf (0.2f, 0.2f, 0.1f, 1.0f),   // matAmbient,
                                       ColorAf (0.5f, 0.3f, 0.2f, 1.0f),   // matDiffuse,
                                       ColorAf (0.2f, 0.12f, 0.08f, 1.0f), // matSpecular,
                                       1.0f));                              // matShininess


  // Create frame buffer
  gl::Fbo::Format format;
  format.enableColorBuffer ();
  format.enableDepthBuffer ();
  format.enableMipmapping (true);
  format.setColorInternalFormat (GL_RGBA32F);

  mBloomFbo = gl::Fbo (mScreenWidth, mScreenHeight, format);

  // --- Misc OpenGL setup ---------------------------------
  glEnable (GL_DEPTH_TEST);
  glDepthMask (GL_TRUE);
}

void GameWorld::update ()
{
  mGravityField->updateModifier ();

  mGravityField->applyGravity ();

  // Create meteors
//  for (int i=0; i<1; i++)
  if (Rand::randFloat(1) < 0.2f)
  {
    Meteor *meteor = new Meteor(10000.0f, 
                                Vec3f(Rand::randFloat(-300, 300), 300, 0),
                                Rand::randFloat(1, 7),
                                mMeteorMaterial);
    meteor->setVelocity(Vec3f(0, -1, 0));
    meteor->setRotationVelocity (Vec3f(Rand::randFloat(3), Rand::randFloat(3), Rand::randFloat(3)));
    mObjects.push_back (meteor);
  }

  // Update basic objects
  for (list<BasicObject *>::iterator it = mObjects.begin(); it != mObjects.end();)
  {
    BasicObject *object = *it;

    object->update();

    if (object->isDead ())
    {
      it = mObjects.erase (it);
      delete object;
    }
    else
      it++;
  }

  // Update particle systems
  ParticleSystemManager::getSingleton().update();
}

void GameWorld::setOrthoProjection ()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-500, 500, -300, 300, -1000.0, 1000.0);
}

void GameWorld::draw   ()
{
	// setup our camera to render the torus scene
  mMovingCamera->setProjectionMatrix ();
  mMovingCamera->setModelMatrix ();


	// clear the FBO
	gl::clear();
  glEnable (GL_DEPTH_TEST);

  mSun->setLightPosition ();

  // Draw the basic objects
  for (list<BasicObject *>::iterator it = mObjects.begin(); it != mObjects.end(); it++)
  {
    BasicObject *object = *it;
    object->draw();
  }

  // Setup blending for particle system
	gl::disableDepthWrite ();
	gl::enableAdditiveBlending();
  ParticleSystemManager::getSingleton().draw ();
	gl::disableAlphaBlending();

  // Draw gravity field
	gl::enableAdditiveBlending();
  mGravityField->draw ();
	gl::disableAlphaBlending();

	gl::enableDepthWrite ();

  // Add blooming effect /////////////////////////

	// bind the framebuffer - now everything we draw will go there
	mBloomFbo.bindFramebuffer();
  	gl::clear ();

    // Draw the basic objects
    for (list<BasicObject *>::iterator it = mObjects.begin(); it != mObjects.end(); it++)
    {
      BasicObject *object = *it;
      object->draw();
    }
	// unbind the framebuffer, so that drawing goes to the screen again
	mBloomFbo.unbindFramebuffer();

  // Get blooming effect
  gl::Fbo& bloomedFbo = mBloomEffect->render (mBloomFbo);

  // Draw blooming effect to screen
  gl::setViewport (getWindowBounds ());
  gl::setMatricesWindow (getWindowSize (), false);
	gl::enableAdditiveBlending();
  gl::color (1, 1, 1, 0.9f);
	gl::draw (bloomedFbo.getTexture(), bloomedFbo.getTexture().getBounds(), getWindowBounds());
	gl::disableAlphaBlending();
}

shared_ptr<BumpMaterial> GameWorld::getBumpMaterial (const TriMesh& mesh,
                                                     const string&  diffuseTexture,
                                                     const string&  normalTexture)
{
	gl::Texture earthColor	= gl::Texture (loadImage (loadFile (diffuseTexture)));
	gl::Texture earthNormal	= gl::Texture (loadImage (loadFile (normalTexture)));
	earthColor.setWrap (GL_REPEAT, GL_REPEAT);
	earthNormal.setWrap (GL_REPEAT, GL_REPEAT);

  gl::GlslProg bumpShader = ShaderHelper::loadShader("../Media/Shaders/BumpMap_Vertex.glsl", "../Media/Shaders/BumpMap_Pixel.glsl");

  BumpMaterial* materialPtr = new BumpMaterial (earthColor, // const gl::Texture&  diffuseTexture,
                                                earthNormal, // const gl::Texture&  normalTexture,
                                                bumpShader, // const gl::GlslProg& shader,
                                                mesh,       // const TriMesh&      mesh, 
                                                ColorAf (1.0f, 1.0f, 1.0f, 1.0f), // const ColorAf&      matAmbient,
                                                ColorAf (1.0f, 1.0f, 1.0f, 1.0f), // const ColorAf&      matDiffuse,
                                                ColorAf (1.0f, 1.0f, 1.0f, 1.0f), // const ColorAf&      matSpecular,
                                                40.0f); // const float         matShininess);

  return shared_ptr<BumpMaterial> (materialPtr);
}
