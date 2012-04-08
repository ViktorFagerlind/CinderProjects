#include "Sun.h"

#include "GameWorld.h"
#include "../../ParticleTest/src/ParticleSystemManager.h"
#include "../../ParticleTest/src/ParticleSystem.h"
#include "../../ParticleTest/src/CommonModifier.h"
#include "../../ParticleTest/src/ColorModifier.h"
#include "../../ParticleTest/src/PointEmitter.h"

Sun::Sun (const float                    mass, 
          const Vec3f&                   position, 
          const float                    radius,   
                gl::VboMesh              vbo)
  : PhysicsObject (mass, position, radius)
{
  // Add to gravity field
  
  GameWorld::getGravityField ()->addGravityObject (this);

  // Initialise light
  float darkgrey[] = {0.3f, 0.3f, 0.3f , 0.3f};
  float white[] = {1.0f, 1.0f, 1.0f , 0.0f};
  float red[]   = {1.0f, 0.0f, 0.0f , 0.0f};
  float yellow[]= {1.0f, 1.0f, 0.0f , 0.0f};
  float green[] = {0.0f, 1.0f, 0.0f , 0.0f};
  float blue[]  = {0.0f, 0.0f, 1.0f , 0.0f};
  float black[] = {0.0f, 0.0f, 0.0f , 0.0f};

	float center[] = {100, 0, 0, 1};

  glLightfv(GL_LIGHT0, GL_SPECULAR, white);
  glLightfv(GL_LIGHT0, GL_DIFFUSE,  white);
  glLightfv(GL_LIGHT0, GL_AMBIENT,  darkgrey);
	glLightfv(GL_LIGHT0, GL_POSITION, center);
	glEnable (GL_LIGHT0);

  mVbo      = vbo;

	try {
		mShader = gl::GlslProg (loadFile ("../Media/Shaders/SunSurface_vert.glsl"), loadFile ("../Media/Shaders/SunSurface_frag.glsl"));
	}	catch (gl::GlslProgCompileExc &exc) {
		std::cout << "Shader compile error: " << std::endl;
		std::cout << exc.what();
	}	catch (...) {
		std::cout << "Unable to load shader" << std::endl;
	}

  // Particle system
  ParticleSystem *sunParticleSystem  = new ParticleSystem("../Media/Images/flare.png");
  
  PointEmitter   *sunEmitter = new PointEmitter (10000,                //maxNofParticles,
                                                 mPosition,            //position, 
  						                                   13,                   //particlesPerFrame, 
							                                   5,                    //minParticleSize,
							                                   15,                   //maxParticleSize,
							                                   Vec3f (0, 0, 0),      //baseVelocity,
                                                 0.5f,
							                                   0.55f);  //randVelocity
                                                 
  CommonModifier* commonModifier = new CommonModifier (1.2f, 1.0f, 1.0f);
  ColorModifier*  colorModifier  = new ColorModifier  (ColorAf(1, 1,    0.5f, 1), //startColor 
                                                       ColorAf(1, 0.8f, 0.2f, 0.3f), //middleColor
                                                       ColorAf(1, 0.6f, 0.1f, 0), //endColor
                                                       0.8f);//float middleTime)
  sunParticleSystem->addModifier (commonModifier);
  sunParticleSystem->addModifier (colorModifier);

  sunParticleSystem->addEmitter (sunEmitter);

  // GameWorld::getParticleSystemManager()->addParticleSystem (sunParticleSystem);
}

void Sun::setLightPosition ()
{
  // Setup sun light
  GLfloat light_position[] = {mPosition.x, mPosition.y, mPosition.z, 1};
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

void Sun::update()
{
}

void Sun::draw()
{
  static float t=0.0f;
  
  t += 0.01f;

	glDisable (GL_BLEND);
  glEnable (GL_TEXTURE_2D);

  // Matrix transforms
  gl::pushModelView();
  gl::translate(mPosition);

  mShader.bind ();
	mShader.uniform ("t", t);

  glEnable (GL_RESCALE_NORMAL);
  gl::scale (mRadius/100.0f, mRadius/100.0f, mRadius/100.0f);

  gl::draw (mVbo);

  glDisable (GL_RESCALE_NORMAL);

  mShader.unbind ();

  gl::popModelView();
}