#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

#include "MSABulletRigidBody.h"
#include "MSABulletRigidWorld.h"

#include "ShaderHelper.h"
#include "PhongMaterial.h"
#include "MovingCamera.h"

using namespace ci;
using namespace ci::app;
using namespace std;

BulletRigidWorld world;

class BulletTestApp : public AppBasic {
public:
	void setup();
	void mouseDown( MouseEvent event );	
  void keyDown (KeyEvent event);
	void update();
	void draw();

private:
  shared_ptr<MovingCamera>  mCamera;
  shared_ptr<PhongMaterial> mMaterial;

  bool mDebugMode;

  uint32_t frameCount;
};

void BulletTestApp::setup()
{
  world.setup(Vec3f(-10, -10, -10), Vec3f(10, 10, 10));
	{
		//create a dynamic rigidbody
		//btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
		btCollisionShape* colShape = new btSphereShape(btScalar(1.));
		//	collisionShapes.push_back(colShape);
		
		/// Create Dynamic Objects
		btTransform startTransform;
		startTransform.setIdentity();
		btScalar	mass(1.f);
		
		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);
		
		btVector3 localInertia(0,0,0);
		if (isDynamic)
			colShape->calculateLocalInertia(mass,localInertia);
		
    startTransform.setOrigin (btVector3 (0,0,0));
		
		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);
		
		world.addBulletRigidBody(body);
	}
	
	Vec3f p(0, 0, 0);
  world.createRigidBox (Vec3f(0.f,-15.f,0.f), Vec3f(15.f, 0.3f, 15.f), 0.f);

  float white[] = {1.0f, 1.0f, 1.0f , 0.0f};
  float red[]   = {1.0f, 0.0f, 0.0f , 0.0f};
  float green[] = {0.2f, 0.8f, 0.3f , 0.0f};
  float blue[]  = {0.0f, 0.0f, 1.0f , 0.0f};
  float black[] = {0.0f, 0.0f, 0.0f , 0.0f};

  float p1[] = {-1000, 5000, 0};

  glLightfv(GL_LIGHT0, GL_SPECULAR, white);
  glLightfv(GL_LIGHT0, GL_DIFFUSE,  white);
  glLightfv(GL_LIGHT0, GL_AMBIENT,  white);
	glLightfv(GL_LIGHT0, GL_POSITION, p1);
	glEnable (GL_LIGHT0);

  mDebugMode = false;

  gl::enableDepthRead ();
  gl::enableDepthWrite ();

  mMaterial.reset (new PhongMaterial (ShaderHelper::loadShader ("../Media/Shaders/phong_vert.glsl", 
                                                                "../Media/Shaders/phong_frag.glsl"), // Shader
                                      ColorAf (0.05f, 0.1f,  0.05f, 1.0f),                           // matAmbient,
                                      ColorAf (0.8f,  0.5f,  0.2f,  1.0f),                           // matDiffuse,
                                      ColorAf (0.9f,  0.5f,  0.3f,  1.0f),                           // matSpecular,
                                      6.0f));                                                        // matShininess

  mCamera.reset (new MovingCamera(50.0f));

  frameCount = 0;
}

void BulletTestApp::mouseDown( MouseEvent event )
{
	for(int i=0; i<10; i++) 
  {
    Vec3f p(Rand::randFloat(-10.f, 10.f), Rand::randFloat(-10.f, 10.f), Rand::randFloat(-10.f, 10.f));

		world.createRigidSphere(p, Rand::randFloat(.5f, 1.f));
	}
}

void BulletTestApp::keyDown (KeyEvent event)
{
  char c = event.getChar();

  if (c == 'd')
  {
    mDebugMode = !mDebugMode;
  }

  mCamera->keyDown (event);
}

void BulletTestApp::update()
{
  world.update();
}

void BulletTestApp::draw()
{
	// clear out the window with black
	gl::clear (Color (0, 0, 0)); 

  // Setup camera
  mCamera->setModelMatrix ();

  if (mDebugMode)
  {
    world.debugDraw();
  }
  else
  {
    mMaterial->bind();

	  for (int j=0; j<world.getBulletWorld().getNumCollisionObjects(); j++)
	  {
		  btCollisionObject* obj = world.getBulletWorld().getCollisionObjectArray()[j];
		  btRigidBody* body = btRigidBody::upcast(obj);
		  if (body && body->getMotionState())
		  {
			  btTransform trans;
			  body->getMotionState()->getWorldTransform(trans);

			  glPushMatrix();
			  glTranslatef(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());

        btCollisionShape *shape = body->getCollisionShape();

        switch (shape->getShapeType ())
        {
        case 0:
          gl::drawCube (Vec3f(0,0,0), btVector3_To_Vec3f (((btBoxShape*) shape)->getHalfExtentsWithoutMargin())); 
          break;
        case 8:
          gl::drawSphere (Vec3f(0,0,0), ((btSphereShape*) shape)->getRadius()); 
          break;
        }

			  glPopMatrix();
		  }
	  }

    mMaterial->unbind();
  }

  frameCount++;
  if ((frameCount % 10) == 0)
    console() << "FPS: " << getAverageFps() << std::endl;
}

CINDER_APP_BASIC( BulletTestApp, RendererGl )
