#include "CollisionDetection.h"

bool CollisionDetection::isCollision (const BoundingGeometry *b1, const BoundingGeometry *b2)
{

  if (b1->mShape == BoundingGeometry::eBox && b2->mShape == BoundingGeometry::ePlane)
    return isCollision ((BoundingBox *)b1, (BoundingPlane *)b2);
  else if (b1->mShape == BoundingGeometry::ePlane && b2->mShape == BoundingGeometry::eBox)
    return isCollision ((BoundingBox *)b2, (BoundingPlane *)b1);
    
  return false;
}

bool CollisionDetection::getCollisionPoint (const BoundingGeometry *b1, const BoundingGeometry *b2, vec3& point, vec3& normal)
{
  if (b1->mShape == BoundingGeometry::eBox && b2->mShape == BoundingGeometry::ePlane)
    return getCollisionPoint ((BoundingBox *)b1, (BoundingPlane *)b2, point, normal);
  else if (b1->mShape == BoundingGeometry::ePlane && b2->mShape == BoundingGeometry::eBox)
    return getCollisionPoint ((BoundingBox *)b2, (BoundingPlane *)b1, point, normal);
    
  return false;
}

bool CollisionDetection::isCollision (const BoundingBox *box, const BoundingPlane *plane)
{
  vec3 dummyPoint;
  vec3 dummyNormal;
  return getCollisionPoint(box, plane, dummyPoint, dummyNormal);
}

bool CollisionDetection::getCollisionPoint (const BoundingBox *box, const BoundingPlane *plane, vec3& point, vec3& normal)
{
  vec3 boxPoints[8];
  box->getVertecies(boxPoints);

  vec3 planeNormal = plane->getNormal();
  float offset = plane->getOffset();

  for (int i=0; i<8; i++)
  {
    if (boxPoints[i].dot(planeNormal) < offset)
    {
      point = boxPoints[i];
      normal = planeNormal;
      return true;
    }
  }
  return false;
}
