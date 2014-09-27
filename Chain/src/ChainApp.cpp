#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"

#include "Chain.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class ChainApp : public AppBasic {
public:
  ~ChainApp ();

  void prepareSettings (Settings *settings);
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();

private:
  ChainElement *ce;

  Vec3f mPosition;
};

ChainApp::~ChainApp()
{
  delete ce;
}

void ChainApp::prepareSettings (Settings *settings)
{
  settings->setWindowSize (1280, 768);
}

void ChainApp::setup()
{
  ce = new ChainElement (Vec3f (500, 100, 0), 0.0f, 100.0f, 10.0f);

  ce->addChild ();
}

void ChainApp::mouseDown( MouseEvent event )
{
}

void ChainApp::update()
{
  Vec2i mousePosition = getMousePos();

  ce->update (Vec3f ((float)mousePosition.x, (float)mousePosition.y, 0.0f));
}

void ChainApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );

  ce->draw ();
}

CINDER_APP_BASIC( ChainApp, RendererGl )
