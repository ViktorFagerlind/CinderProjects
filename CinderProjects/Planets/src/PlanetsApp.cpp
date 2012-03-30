#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"

#include "GameWorld.h"

using namespace ci::app;

class PlanetsApp : public AppBasic {
  public: 
  void prepareSettings (Settings *settings);
	void setup ();
	void mouseDown (MouseEvent event);	
	void keyDown (KeyEvent event);
	void update ();
	void draw ();

};

void PlanetsApp::prepareSettings (Settings *settings)
{
  settings->setWindowSize (GameWorld::mScreenWidth, GameWorld::mScreenHeight);
}

void PlanetsApp::setup()
{
  GameWorld::getSingleton ()->setup ();
}

void PlanetsApp::mouseDown (MouseEvent event)
{
}

void PlanetsApp::keyDown (KeyEvent event)
{
  GameWorld::getSingleton ()->keyDown (event);
}

void PlanetsApp::update()
{
  GameWorld::getSingleton ()->update ();
}

void PlanetsApp::draw()
{
  GameWorld::getSingleton ()->draw ();
}

CINDER_APP_BASIC( PlanetsApp, RendererGl )
