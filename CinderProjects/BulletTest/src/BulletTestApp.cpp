#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include "cinder/Camera.h"


#include "MSABulletRigidBody.h"
#include "MSABulletRigidWorld.h"

using namespace ci;
using namespace ci::app;
using namespace std;

BulletRigidWorld world;

class BulletTestApp : public AppBasic {
public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();

private:
  CameraPersp mCam;

};

void BulletTestApp::setup()
{
  world.setup(Vec3f(0, 0, 0), Vec3f(getWindowWidth(), getWindowHeight(), 0));

	{
		
		//create a dynamic rigidbody
		//btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
		btCollisionShape* colShape = new btSphereShape(btScalar(10.));
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
		
    startTransform.setOrigin(btVector3(getWindowWidth() /2,getWindowHeight()/2,0));
		
		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);
		
		world.addBulletRigidBody(body);
	}
	
	Vec3f p(700, 200, 0);
	world.createRigidSphere(p, 10);


  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glEnable( GL_LIGHTING );

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
}

void BulletTestApp::mouseDown( MouseEvent event )
{
	for(int i=0; i<10; i++) 
  {
    Vec3f p(Rand::randInt(0, getWindowWidth()), Rand::randInt(0, getWindowHeight()), 0);
		world.createRigidSphere(p, Rand::randInt(10, 50));
	}
}

void BulletTestApp::update()
{
  world.update();
}

void BulletTestApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 

  /*
  Vec3f cameraPosition = Vec3f (0.0f, 0.0f, 1000.0f);

  mCam.lookAt(cameraPosition,  // camera
              Vec3f::zero(),   // center
              Vec3f::yAxis()); // up

  gl::setMatrices (mCam);
  */
  world.debugDraw();

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
			float radius = ((btSphereShape*) body->getCollisionShape())->getRadius();
      gl::drawSphere (Vec3f(0,0,0), radius); 
			glPopMatrix();
		}
	}
}

CINDER_APP_BASIC( BulletTestApp, RendererGl )
