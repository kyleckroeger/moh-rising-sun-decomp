// Fragment of cdbgeom.cpp: a CDB triangle tests an animated volume as a sphere.
#include "CDBGeom.h"
#include "AnimatedVolume.h"

EClsnState CCDBTriangle::TestCollision(const CAnimatedVolume &other, CCollision &collision, const CDBTri *triangle,
                                       bool flag) const {
    return TestCollision((const CVolSphere &)other, collision, triangle, flag);
}
