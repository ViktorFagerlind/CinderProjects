#include "cinder/app/AppNative.h"
#include "cinder/app/App.h"
#include "cinder/gl/Light.h"

class Protagonist;
class MovingCamera;

using namespace ci;
using namespace ci::app;
using namespace std;

class SpacewarsApp : public AppNative
{
public:
  void prepareSettings (Settings *settings);
	void setup();

	void mouseDown(MouseEvent event);	
	void mouseUp(MouseEvent event);
	void mouseMove(MouseEvent event);
	void mouseDrag(MouseEvent event);

	void touchesBegan( TouchEvent event );
	void touchesMoved( TouchEvent event );
	void touchesEnded( TouchEvent event );

	void keyDown(KeyEvent event);
	void update();
	void draw();

  void setupLights ();

private:
	float m_width;
	float m_height;

	bool  m_touchPressed;
	vec2 m_touchPosition;
	vec2 m_touchVelocity;
};

