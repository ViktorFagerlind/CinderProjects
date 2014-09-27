#include "cinder/Vector.h"

using namespace ci;

class ChainElement
{
public:
  ChainElement (Vec3f position, float angle, float length, float mass);

  ~ChainElement ();

  void addChild ();

  void update (const Vec3f& position);

  void draw ();

  float getLineForce ();

  Vec3f getEndPosition ();

public:
  static float g;

private:
  Vec3f mPosition;
  float mAngle;

//  float mAngleAcceleration;
  float mAngleSpeed;

  float mLength;
  float mMass;

  ChainElement *mChild;
};


