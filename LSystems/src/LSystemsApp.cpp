#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/Vector.h"

#include "LSystem.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class TreeDrawState
{
public:
  Vec3f position;
  float angle;
  float segmentLength;
  float deltaAngle;
};

class TreeRenderer : public LSystemRenderer
{
public:
  void draw (const string& lSystemString)
  {
    list<TreeDrawState> savedStates;
    TreeDrawState       currentState;

    currentState.position      = Vec3f (640,700,0);
    currentState.angle         = 0.0f;
    currentState.segmentLength = 5.0f;
    currentState.deltaAngle    = 20.0f * (2.0f * 3.14f)/360.0f;

    Vec3f previousPosition = currentState.position;

    for (string::const_iterator it = lSystemString.begin (); it != lSystemString.end (); it++)
    {
      char c = *it;

      switch (c)
      {
        case 'f': // move to the next position without drawing a segment
        {
          previousPosition       = currentState.position;

          Vec3f unitSegment = Vec3f (0,-currentState.segmentLength,0);
          unitSegment.rotateZ (currentState.angle);
          currentState.position += unitSegment;
          break;
        }
        case 'F': // the system draws a segment here (a rectangle by default).
        {
          previousPosition       = currentState.position;

          Vec3f unitSegment = Vec3f (0,-currentState.segmentLength,0);
          unitSegment.rotateZ (currentState.angle);
          currentState.position += unitSegment;

          gl::drawLine (previousPosition, currentState.position);
          break;
        }
        case '+': // rotate counterclockwise by lsystem.angle.
          currentState.angle -= currentState.deltaAngle;
          break;
        case '-': // rotate clockwise by lsystem.angle.
          currentState.angle += currentState.deltaAngle;
          break;
        case '|': // rotate 180 degrees.
          currentState.angle += 3.14f;
          break;
        case '[': // works like the push() command, starting a subbranch.
          savedStates.push_back (currentState);
          break;
        case ']': // works like the pop() command, ending the subbranch.
          currentState    = savedStates.back ();
          savedStates.pop_back ();
          break;
        case '!': // reverses the rotation angle.
          currentState.deltaAngle = -currentState.deltaAngle;
          break;
        case '(': // increases the rotation angle to 110%.
          currentState.deltaAngle *= 1.1f;
          break;
        case ')': // decreases the rotation angle to 90%.
          currentState.deltaAngle *= 0.9f;
          break;
        case '<': // increases the segment length to 110%.
          currentState.segmentLength *= 1.1f;
          break;
        case '>': // decreases the segment length to 90%.
          currentState.segmentLength *= 0.9f;
          break;
      }
    }
  }
};


class LSystemsApp : public AppBasic {
  public:
	void prepareSettings (Settings *settings);
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();

private:
  unsigned int mFrameCount;

  TreeRenderer   treeRenderer;

  LSystemRule    *rule1;
  LSystemRule    *rule2;
  LSystemRule    *rule3;
  LSystemRuleSet ruleSet;
  LSystem        *ls;
};

void LSystemsApp::prepareSettings (Settings *settings)
{
  settings->setWindowSize (1280, 768);
}

void LSystemsApp::setup()
{
  //rule1 = new LSystemRule('1', "FF-[1]++F+F+1");

/*
  rule1 = new LSystemRule('1', "FF[-2]3[+3]");
  rule2 = new LSystemRule('2', "FF+F-F-F[FFF3][+3]-F-F3");
  rule3 = new LSystemRule('3', "FF-F+F+F[2][-2]+F+F2");
*/

/*
  rule1 = new LSystemRule('1',  "FFF-[-F+F[2]-[1]]+[+F+F[1]-[1]]");
  rule2 = new LSystemRule('2',  "FF-[-F+F]+[+F+F2]");
*/


  rule1 = new LSystemRule('X', "F[+>X]+F[->X]F-[+>>X]-F[->>X]F>X");
  rule2 = new LSystemRule('F', "FF");

//  rule1 = new LSystemRule('X', "C0F-[C2[X]+C3X]+C1F[C3+FX]-X");
//  rule2 = new LSystemRule('F', "FF");

  // Triangles
//  rule1 = new LSystemRule('A', "BF-AF-B");
//  rule2 = new LSystemRule('B', "AF+BF+A");

//  rule1 = new LSystemRule('F', "FF-[-F+F+F]+[+F-F-F]");

  ruleSet.addRule (rule1);
  ruleSet.addRule (rule2);
//  ruleSet.addRule (rule3);

  ls = new LSystem ("X", &ruleSet, &treeRenderer);
}

void LSystemsApp::mouseDown( MouseEvent event )
{
  if (event.isLeft ())
    ls->update ();
  else
    ls->reset ();

  gl::clear( Color( 0, 0, 0 ) ); 

  ls->draw ();
}

void LSystemsApp::update()
{
}

void LSystemsApp::draw()
{
}

CINDER_APP_BASIC( LSystemsApp, RendererGl )
