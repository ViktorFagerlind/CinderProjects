#include "Planet.h"
#include "math.h"
#include "GravityField.h"
#include "GameWorld.h"

static const float density = 1000;

//----------------------------------------------------------------------------------------------------------------------

Planet::Planet (const float           a, 
                const float           e, 
                const float           radius, 
                const float           initialVelocity, 
                const Vec3f&          color, 
                const PhysicsObject&  gravityCenter, 
                const string&         textureFile)
: PhysicsObject (4.0f/3.0f * (float)M_PI * radius * radius * radius * density,
                 Vec3f(0, 0, 0),
                 radius),
  mGravityCenter (gravityCenter)
{
  float trackRadius = a * (1.0f - e);

  mL                = mMass * initialVelocity * trackRadius;

  mA      = a;
  mE      = e;
  mTheta  = 0.0f;

  mColor = color;

  mTexture = new gl::Texture (loadImage (loadFile (textureFile)));

  GameWorld::getGravityField ()->addGravityObject (this);
}

//----------------------------------------------------------------------------------------------------------------------

/* eq. 1: r = a (1-e^2)/(1+e*cos(eta))
 * eq. 2: L = m*v*r  
 * eq. 3: deltaTheta = v/r
 */
void Planet::update()
{
  float r           = mA * (1.0f - mE*mE)/(1.0f+mE*cos(mTheta)); // (1)
  float v           = mL /(mMass*r);                       // (2)
  float deltaTheta  = v / r;                               // (3)

  mTheta    = mTheta + deltaTheta;
  mPosition = mGravityCenter.getPosition () + Vec3f(r * cos(mTheta), r * sin(mTheta), 0.0f);
}

//----------------------------------------------------------------------------------------------------------------------

void Planet::draw()
{
	glEnable (GL_TEXTURE_2D);
  mTexture->bind();

  glColor3f (mColor.x, mColor.y, mColor.z);
  gl::drawSphere (mPosition, mRadius, 20);

  mTexture->unbind();
}



