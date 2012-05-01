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
  float ambient[]  = {0.3f, 0.3f, 0.3f, 1.0f};
  float specular[] = {1.5f, 1.5f, 1.5f, 1.0f};
  float diffuse[]  = {1.0f, 1.0f, 1.0f, 1.0f};

	float center[] = {100, 0, 0, 1};

  glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
  glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffuse);
  glLightfv(GL_LIGHT0, GL_AMBIENT,  ambient);
	glLightfv(GL_LIGHT0, GL_POSITION, center);
	glEnable (GL_LIGHT0);

  mVbo      = vbo;

	try {
		mShader = gl::GlslProg (loadFile ("../Media/Shaders/passThru_vert.glsl"), loadFile ("../Media/Shaders/SunSurface_frag.glsl"));
	}	catch (gl::GlslProgCompileExc &exc) {
		std::cout << "Shader compile error: " << std::endl;
		std::cout << exc.what();
	}	catch (...) {
		std::cout << "Unable to load shader" << std::endl;
	}

  // Particle system
  mSunParticleSystem  = new ParticleSystem("../Media/Images/flare.png");
  
  PointEmitter   *sunEmitter = new PointEmitter (10000,                //maxNofParticles,
                                                 mPosition,            //position, 
  						                                   50,                   //particlesPerFrame, 
							                                   5,                    //minParticleSize,
							                                   15,                   //maxParticleSize,
							                                   Vec3f (0, 0, 0),      //baseVelocity,
                                                 0.8f,
							                                   0.9f);  //randVelocity
                                                 
  CommonModifier* commonModifier = new CommonModifier (1.2f, 2.0f, 0.1f);
  ColorModifier*  colorModifier  = new ColorModifier  (ColorAf(1, 1,    0.5f, 0.0f), //startColor 
                                                       ColorAf(1, 0.8f, 0.2f, 0.2f), //middleColor
                                                       ColorAf(1, 0.6f, 0.1f, 0), //endColor
                                                       0.8f);//float middleTime)
  mSunParticleSystem->addModifier (commonModifier);
  mSunParticleSystem->addModifier (colorModifier);
  mSunParticleSystem->addEmitter (sunEmitter);
}

Sun::~Sun()
{
  delete mSunParticleSystem;
}


void Sun::setLightPosition ()
{
  // Setup sun light
  GLfloat light_position[] = {mPosition.x, mPosition.y, mPosition.z, 1};
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

void Sun::update()
{
  mSunParticleSystem->update ();
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
  gl::scale (mRadius, mRadius, mRadius);

  gl::draw (mVbo);

  glDisable (GL_RESCALE_NORMAL);

  mShader.unbind ();

  gl::popModelView();


  // Draw particles
	gl::disableDepthWrite ();
	gl::enableAdditiveBlending();

  mSunParticleSystem->draw ();

	gl::disableAlphaBlending();
	gl::enableDepthWrite ();
}