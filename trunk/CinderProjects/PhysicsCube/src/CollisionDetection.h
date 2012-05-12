#pragma once
#include "BoundingPlane.h"
#include "BoundingBox.h"
#include "BoundingSphere.h"
#include "BoundingGeometry.h"
#include "cinder/Vector.h"

class CollisionDetection
{
public:
  static bool isCollision (const BoundingGeometry *b1, const BoundingGeometry *b2);
  static bool getCollisionPoint (const BoundingGeometry *b1, const BoundingGeometry *b2, Vec3f& point, Vec3f& normal);
  
private:
  static bool isCollision (const BoundingBox *box, const BoundingPlane *plane);
  static bool getCollisionPoint (const BoundingBox *box, const BoundingPlane *plane, Vec3f& point, Vec3f& normal);



};