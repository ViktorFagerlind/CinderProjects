#include "cinder/app/AppBasic.h"
#include "cinder/cairo/Cairo.h"
#include "cinder/Rand.h"
#include "cinder/ImageIo.h"
#include "cinder/Utilities.h"

#include "MyStrings.h"

using namespace ci;
using namespace ci::app;

#include <vector>
using std::vector;

#include "Sprout.h"

const int MAX_BRANCHES = 40;
const int IDLE_RESTART_FRAMES = 72;
const int WIDTH   = 1280;
const int HEIGHT  = 768;

class VegetationApp : public AppBasic 
{
public:
  ~VegetationApp();

  void prepareSettings( Settings *settings );
  void setup();

  void renderScene( cairo::Context &ctx );
  void keyDown( KeyEvent event );
  void mouseDown( MouseEvent event );

  void clearBranches();
  void clearVegetation();

  void update();
  void draw();

  cairo::SurfaceImage	mOffscreenBuffer;
  cairo::Context			mOffscreenContext;

  vector<Branch*>	mBranches;

  bool mSavingImages;
};

VegetationApp::~VegetationApp()
{
  clearBranches();
}

void VegetationApp::prepareSettings( Settings *settings )
{
  settings->setWindowSize( WIDTH, HEIGHT );
  settings->setResizable( false );
}

void VegetationApp::setup()
{
  // allocate our offscreen buffer
  mOffscreenBuffer  = cairo::SurfaceImage( WIDTH, HEIGHT, true );
  mOffscreenContext = cairo::Context( mOffscreenBuffer );

  // Let the Sproutes know how big our windows is
  Sprout::setWindowSize( WIDTH, HEIGHT );

  // Clear screen
  clearVegetation();

  mSavingImages = false;
}

void VegetationApp::renderScene( cairo::Context &ctx )
{
  for( vector<Branch*>::iterator it = mBranches.begin(); it != mBranches.end(); it++ ) 
    (*it)->draw( ctx );
}

void VegetationApp::clearBranches()
{
  // clear the context
  for( vector<Branch*>::iterator it = mBranches.begin(); it != mBranches.end(); it++ ) 
    delete *it;

  mBranches.clear();
}

void VegetationApp::clearVegetation()
{
  // clear the context
  mOffscreenContext.setSourceRgba( 0.0f, 0.0f, 0.0f, 1.0f );
  mOffscreenContext.paint();

  clearBranches();
}

void VegetationApp::keyDown( KeyEvent event )
{
  switch( event.getChar() ) 
  {
    case 'p':
      writeImage ("Vegetation.png", copyWindowSurface());
      break;

    case 's':
      mSavingImages = !mSavingImages;
      break;
  }
}

void VegetationApp::mouseDown( MouseEvent event )
{
  if (event.isLeft())
    mBranches.push_back (new Branch (event.getPos(), Sprout::randomHue()));
  else
    clearVegetation ();
}

void VegetationApp::update()
{
  // Iterate all the blossoms, update each one and notice if they are all idle
  for (vector<Branch*>::iterator it = mBranches.begin(); it != mBranches.end();) 
  {
    Branch *branch = *it;

    if (!branch->isAlive())
    {
      delete branch;
      it = mBranches.erase (it);
    }
    else
    {
      branch->update();
      it++;
    }
  }
}

void VegetationApp::draw()
{
  cairo::Context ctx( cairo::createWindowSurface() );
  renderScene( mOffscreenContext );
  ctx.copySurface( mOffscreenBuffer, mOffscreenBuffer.getBounds() );

  if (mSavingImages)
  {
    static int currentImage = 0;
    if ((currentImage%2) == 0)
  	  writeImage (MyString::getFrameNumber ("saveImage_", currentImage/2) + ".png", copyWindowSurface());	
	  currentImage++;
  }
}

CINDER_APP_BASIC( VegetationApp, Renderer2d )
