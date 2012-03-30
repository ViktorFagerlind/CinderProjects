#include "GameWorld.h"

#include "cinder/gl/gl.h"

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

  mMovingCamera = new MovingCamera (GameWorld::mScreenWidth, GameWorld::mScreenHeight);

  // --- Initialise planet system ---------------------------------

  mCenterObject = new PhysicsObject (100.0f*1000.0f*1000.0f, Vec3f (100, 0, 0), 30);
  mGravityField = new GravityField (Vec3f (-300, -300, -300), Vec3f (600, 600, 600), Vec3i (30, 30, 9));

  mGravityField->addGravityObject (mCenterObject);

  const string planetTextures[4] = {"jupiter.jpg", "mars.jpg", "moon.jpg", "earth.jpg"};


  for (int i=0; i<7; i++)
  {
    Planet *planet = new Planet(Rand::randFloat(50, 350),    // a
                                Rand::randFloat(0.5f, 0.99f),  // e
                                Rand::randFloat(5, 20),       // radius
                                Rand::randFloat(0.5f, 8),    // initial velocity
                                Vec3f (1,1,1),// Vec3f (Rand::randFloat(), Rand::randFloat(), Rand::randFloat()),
                                *mCenterObject,// center 
                                "../Media/Images/" + planetTextures[Rand::randInt (4)]);              
    
    mObjects.push_back (planet);

    for (int j = 0; j<2; j++)
    {
      Planet *moon = new Planet(planet->getRadius() + Rand::randFloat(10, 30),      // a
                                Rand::randFloat(0.0f, 0.5f),  // e
                                planet->getRadius() * 0.3f,   // radius
                                Rand::randFloat(1, 2),       // initial velocity
                                Vec3f (1,1,1),//Vec3f (Rand::randFloat(), Rand::randFloat(), Rand::randFloat()),
                                *planet,              // center 
                                "../Media/Images/" + planetTextures[Rand::randInt (4)]);
     mObjects.push_back (moon);
    }
  }

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

//  PerlinModifier *perlinModifier = new PerlinModifier (100, 0.06f, 0.1f);
//  particleSystem->addModifier (perlinModifier);

  
  commonModifier = new CommonModifier (1, 1.0f, 0.5f);
  particleSystem->addModifier (commonModifier);
    
  // --- The explosions -------------------
  mExposionSystem = new ParticleSystem ("../Media/Images/fire.png");
  commonModifier = new CommonModifier (3, 1.0f, 0.5f);
  colorModifier  = new ColorModifier (ColorAf(1, 1, 1, 1), //startColor 
                                      ColorAf(1, 1, 1, 1), //middleColor
                                      ColorAf(1, 1, 1, 0), //endColor
                                      0.5f);//float middleTime)

  mExposionSystem->addModifier (commonModifier);
  mExposionSystem->addModifier (colorModifier);

  mParticleSystemManager->addParticleSystem (mExposionSystem);

  // --- The sun -------------------
  ParticleSystem *sunParticleSystem  = new ParticleSystem("../Media/Images/flare.png");
  
  PointEmitter   *sunEmitter = new PointEmitter (10000,                 //maxNofParticles,
                                                 mCenterObject->getPosition(),//position, 
  						                                   10,                   //particlesPerFrame, 
							                                   5,                    //minParticleSize,
							                                   15,                   //maxParticleSize,
							                                   Vec3f (0, 0, 0),            //baseVelocity,
							                                   Vec3f (0.6f, 0.6f, 0.6f));  //randVelocity
                                                 
  commonModifier = new CommonModifier (1.2f, 1.0f, 0.5f);
  colorModifier  = new ColorModifier (ColorAf(1, 1, 1, 1), //startColor 
                                      ColorAf(1, 1, 0.5f, 1), //middleColor
                                      ColorAf(1, 0.6f, 0.3f, 0), //endColor
                                      0.8f);//float middleTime)
  sunParticleSystem->addModifier (commonModifier);
  sunParticleSystem->addModifier (colorModifier);

  sunParticleSystem->addEmitter (sunEmitter);
  mParticleSystemManager->addParticleSystem (sunParticleSystem);

  // --- Initialise lights ---------------------------------
  
  glEnable (GL_COLOR_MATERIAL);
  glColorMaterial (GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glEnable (GL_LIGHTING);

  float white[] = {1.0f, 1.0f, 1.0f , 0.0f};
  float red[]   = {1.0f, 0.0f, 0.0f , 0.0f};
  float yellow[]= {1.0f, 1.0f, 0.0f , 0.0f};
  float green[] = {0.0f, 1.0f, 0.0f , 0.0f};
  float blue[]  = {0.0f, 0.0f, 1.0f , 0.0f};
  float black[] = {0.0f, 0.0f, 0.0f , 0.0f};

	float center[] = {0, 0, 0, 1};

  glLightfv(GL_LIGHT0, GL_SPECULAR, white);
  glLightfv(GL_LIGHT0, GL_DIFFUSE,  yellow);
  glLightfv(GL_LIGHT0, GL_AMBIENT,  black);
	glLightfv(GL_LIGHT0, GL_POSITION, center);
	glEnable (GL_LIGHT0);

  /*
  glLightfv(GL_LIGHT1, GL_SPECULAR, white);
  glLightfv(GL_LIGHT1, GL_DIFFUSE,  blue);
  glLightfv(GL_LIGHT1, GL_AMBIENT,  black);
	glLightfv(GL_LIGHT1, GL_POSITION, Vec3f (0, -500,0));
	glEnable (GL_LIGHT1);

  glLightfv(GL_LIGHT2, GL_SPECULAR, white);
  glLightfv(GL_LIGHT2, GL_DIFFUSE,  green);
  glLightfv(GL_LIGHT2, GL_AMBIENT,  black);
	glLightfv(GL_LIGHT2, GL_POSITION, Vec3f (0, 0,0));
	glEnable (GL_LIGHT2);*/

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
  // Update camera
  mMovingCamera->setViewMatrix ();

  // Clear the window
	gl::clear (Color (0, 0, 0));

  // Setup material for planets
  float mat_ambient[]		 = {0.6f, 0.3f, 0.4f, 1.0f};
  float mat_diffuse[]		 = {0.3f, 0.5f, 0.8f, 1.0f};
  float mat_specular[]	 = {1.0f, 1.0f, 1.0f, 1.0f};
  glMaterialfv (GL_FRONT, GL_SHININESS, mat_ambient);
  glMaterialfv (GL_FRONT, GL_DIFFUSE,	  mat_diffuse);
	glMaterialfv (GL_FRONT, GL_AMBIENT,	  mat_ambient);

  // Setup sun light
	GLfloat light_position[] = {0, 0, 0, 1};
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

  // Setup lighting for planets
	glDisable (GL_BLEND);
  glEnable  (GL_LIGHTING);

  // Draw the basic objects
  for (list<BasicObject *>::iterator it = mObjects.begin(); it != mObjects.end(); it++)
  {
    BasicObject *object = *it;
    object->draw();
  }

  // Draw the sun
	glDisable (GL_LIGHTING);
  glColor3f (1, 1, 1);
  gl::drawSphere (mCenterObject->getPosition(), mCenterObject->getRadius (), 30);

  // Setup blending for particle system
  glDisable (GL_LIGHTING);
	glEnable  (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE);

//  glDisable (GL_DEPTH_TEST);
  glDepthMask (false);

  // Draw particle systems
  mParticleSystemManager->draw ();
  glDepthMask (true);

  // Draw gravity field
  mGravityField->draw ();
}