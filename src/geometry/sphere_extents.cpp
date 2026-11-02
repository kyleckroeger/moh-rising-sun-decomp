// Fragment of sphere.cpp: the bounding box is the center less and plus the radius.
#include "Sphere.h"


void CVolSphere::GetExtents(CVector3 &low, CVector3 &high) const {
    const float r = radius;
    CVector3 extent(r, r, r);
    CVector3 &e = extent;
    const CVector3 &c = center;
    low.x = center.x - e.x;
    low.y = c.y - e.y;
    low.z = c.z - e.z;
    high.x = center.x + e.x;
    high.y = c.y + e.y;
    high.z = c.z + e.z;
}
