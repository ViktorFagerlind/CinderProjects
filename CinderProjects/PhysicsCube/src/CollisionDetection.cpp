#include "CollisionDetection.h"

bool CollisionDetection::isCollision (const BoundingGeometry *b1, const BoundingGeometry *b2)
{

  if (b1->mShape == BoundingGeometry::eBox && b1->mShape == BoundingGeometry::ePlane)
    return isCollision ((BoundingBox *)b1, (BoundingPlane *)b2);
  else if (b1->mShape == BoundingGeometry::ePlane && b1->mShape == BoundingGeometry::eBox)
    return isCollision ((BoundingBox *)b2, (BoundingPlane *)b1);

  return false;
}

void CollisionDetection::getCollision (const BoundingGeometry *b1, const BoundingGeometry *b2, Vec3f& point, Vec3f& normal)
{
}


bool CollisionDetection::isCollision (const BoundingBox *box, const BoundingPlane *plane)
{
  Vec3f boxPoints[8];
  box->getVertecies(boxPoints);

  Vec3f planeNormal = plane->getNormal();
  float offset = plane->getOffset();

  for (int i=0; i<8; i++)
  {
    if (boxPoints[i].dot(planeNormal) > offset)
    {
    return false;
    }
  }
  return true;
}
