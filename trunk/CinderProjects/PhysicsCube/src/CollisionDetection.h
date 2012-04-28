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
  static void getCollision (const BoundingGeometry *b1, const BoundingGeometry *b2, Vec3f& point, Vec3f& normal);

private:
  static bool isCollision (const BoundingBox *box, const BoundingPlane *plane);



};
