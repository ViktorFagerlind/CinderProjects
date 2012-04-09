#include "GameWorld.h"

#include "cinder/gl/gl.h"
#include "cinder/gl/Vbo.h"
#include "cinder/ObjLoader.h"

#include "Planet.h"
#include "Meteor.h"

#include "../../ParticleTest/src/AreaEmitter.h"
#include "../../ParticleTest/src/CommonModifier.h"
#include "../../ParticleTest/src/ColorModifier.h"
#include "../../ParticleTest/src/PerlinModifier.h"
#include "../../ParticleTest/src/PointEmitter.h"

#include <list>

GameWorld* GameWorld::theGameWorld = NULL;

unsigned int GameWorld::mScreenWidth  = 1600;
unsigned int GameWorld::mScreenHeight = 1000;


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

void GameWorld::setup ()
{
  CommonModifier *commonModifier;
  ColorModifier  *colorModifier;

  mBloomEffect = new BloomEffect (mScreenWidth/4, mScreenHeight/4, mScreenWidth, mScreenHeight);

  mMovingCamera = new MovingCamera (GameWorld::mScreenWidth, GameWorld::mScreenHeight);

  // Setup gravity field
  mGravityField = new GravityField (Vec3f (-300, -300, -300), Vec3f (600, 600, 600), Vec3i (30, 30, 9));

  // --- Initialise particle system ---------------------------------
  mParticleSystemManager = new ParticleSystemManager ();

  // --- The star dust -------------------
  ParticleSystem *particleSystem  = new ParticleSystem("../Media/Images/particle1.jpg");
  mParticleSystemManager->addParticleSystem (particleSystem);

  AreaEmitter *areaEmitter     = new AreaEmitter  (50000,          // maxNofParticles,
                                                   Vec3f (0, 0, 0), // position, 
                                                   10.0f,          // particlesPerFrame,
  						                                     650,             // width, 
							                                     600,             // height,
							                                     200,             // depth,
							                                     0.7f,               // minParticleSize,
							                                     0.7f,               // maxParticleSize,
							                                     0,           // minParticleVelocity,
							                                     0);           // maxParticleVelocity
  particleSystem->addEmitter (areaEmitter); 

  particleSystem->addModifier (mGravityField);

  commonModifier = new CommonModifier (1, 1.0f, 0.5f);
  particleSystem->addModifier (commonModifier);
    
  // --- The explosions -------------------
  mExposionSystem = new ParticleSystem ("../Media/Images/fire.png");
  commonModifier = new CommonModifier (3, 1.0f, 0.5f);
  colorModifier  = new ColorModifier (ColorAf(1, 1, 1, 0.1f), //startColor 
                                      ColorAf(1, 1, 1, 0.1f), //middleColor
                                      ColorAf(1, 1, 1, 0.0f), //endColor
                                      0.5f);//float middleTime)

  mExposionSystem->addModifier (commonModifier);
  mExposionSystem->addModifier (colorModifier);

  mParticleSystemManager->addParticleSystem (mExposionSystem);

  // --- Initialise planet system ---------------------------------

  // Load mesh
  ObjLoader loader (loadFile ("../Media/Meshes/Sphere.obj"));
  TriMesh      planetMesh;
  loader.load (&planetMesh);
	gl::VboMesh planetVbo = gl::VboMesh (planetMesh);


  mSun = new Sun (100.0f*1000.0f*1000.0f, Vec3f (100, 0, 0), 30, planetVbo);
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

  vector<shared_ptr<BumpMaterial>> materials;
  materials.push_back (brickMaterial);
  materials.push_back (earthMaterial);
  materials.push_back (ownMaterial);
  materials.push_back (greenMaterial);
  materials.push_back (moonMaterial);

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

  // Create frame buffer
  gl::Fbo::Format format;
  format.enableColorBuffer ();
  format.enableDepthBuffer ();
  format.enableMipmapping (true);
  format.setColorInternalFormat (GL_RGBA32F);

  mRenderFbo = gl::Fbo (mScreenWidth, mScreenHeight, format);

  // --- Misc OpenGL setup ---------------------------------
  glEnable (GL_DEPTH_TEST);
  glDepthMask (GL_TRUE);
}

void GameWorld::update ()
{
  mGravityField->update ();

  mGravityField->applyGravity ();

  // Create meteors
//  for (int i=0; i<1; i++)
  if (Rand::randFloat(1) < 0.2f)
  {
    Meteor *meteor = new Meteor(10000.0f, 
                                Vec3f(Rand::randFloat(-300, 300), 300, 0),
                                Rand::randFloat(1, 7));
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
  mParticleSystemManager->update();
}

void GameWorld::draw   ()
{
	// bind the framebuffer - now everything we draw will go there
	mRenderFbo.bindFramebuffer();

	// setup the viewport to match the dimensions of the FBO
	gl::setViewport( mRenderFbo.getBounds() );

	// setup our camera to render the torus scene
  mMovingCamera->setViewMatrix ();

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
	glEnable  (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE);
	gl::disableDepthWrite ();

  // Draw particle systems
  mParticleSystemManager->draw ();

	gl::enableDepthWrite ();
	glDisable  (GL_BLEND);

  // Draw gravity field
  /*
	glEnable  (GL_BLEND);
  mGravityField->draw ();
	glDisable  (GL_BLEND);
  */

	// unbind the framebuffer, so that drawing goes to the screen again
	mRenderFbo.unbindFramebuffer();

  ///////////////////////////
 
	gl::clear ();

  // Draw normal scene to screen
  gl::disableDepthRead ();
  gl::color (1, 1, 1, 1);
  gl::setMatricesWindow (getWindowSize (), false);
	gl::draw (mRenderFbo.getTexture(), mRenderFbo.getTexture().getBounds(), getWindowBounds());

  // Get blooming effect
  gl::Fbo& bloomedFbo = mBloomEffect->render (mRenderFbo);

  // Draw particle systems
  /*
  gl::setViewport (getWindowBounds ());
  mMovingCamera->setViewMatrix ();
  mParticleSystemManager->draw ();
  */

  // Add blooming effect to screen
  gl::setMatricesWindow (getWindowSize (), false);
	gl::enableAdditiveBlending();
  gl::color (1, 1, 1, 1);
	gl::draw (bloomedFbo.getTexture(), bloomedFbo.getTexture().getBounds(), getWindowBounds());
	gl::disableAlphaBlending();
}

shared_ptr<BumpMaterial> GameWorld::getBumpMaterial (const TriMesh& mesh,
                                                     const string&  diffuseTexture,
                                                     const string&  normalTexture)
{
	gl::Texture earthColor	= gl::Texture (loadImage (loadFile (diffuseTexture)));
	gl::Texture earthNormal	= gl::Texture (loadImage (loadFile (normalTexture)));
	earthColor.setWrap( GL_REPEAT, GL_REPEAT );
	earthNormal.setWrap( GL_REPEAT, GL_REPEAT );

  gl::GlslProg bumpShader;
	try {
		bumpShader = gl::GlslProg (loadFile ("../Media/Shaders/BumpMap_Vertex.glsl"), loadFile ("../Media/Shaders/BumpMap_Pixel.glsl"));
	}	catch (gl::GlslProgCompileExc &exc) {
		std::cout << "Shader compile error: " << std::endl;
		std::cout << exc.what();
	}	catch (...) {
		std::cout << "Unable to load shader" << std::endl;
	}

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
