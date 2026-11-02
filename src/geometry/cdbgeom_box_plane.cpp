// Fragment of cdbgeom.cpp: a CDB box tests a plane by the box's radius along the normal.
#include "CDBGeom.h"
#include "Frustum.h"
#include <math.h>

static inline float DotOf(const CVector3 &a, const CVector3 &b) {
    float sum = a.x * b.x + a.y * b.y;
    sum += a.z * b.z;
    return sum;
}

EClsnState CCDBVolBox::TestCollision(const CPlane &plane, CCollision &, bool) const {
    CVector3 proj;
    CVector3 &p = proj;
    p.Set(DotOf(axisX, plane.normal), DotOf(axisY, plane.normal), DotOf(axisZ, plane.normal));
    p.x = (float)fabs(p.x);
    p.y = (float)fabs(p.y);
    p.z = (float)fabs(p.z);
    CVector3 half(halfSize[0], halfSize[1], halfSize[2]);
    float radius = DotOf(p, half);
    float distance = DotOf(plane.normal, center) - plane.distance - radius;
    int result = 2;
    if (!(distance < 0.0f))
        result = 1;
    return (EClsnState)result;
}
