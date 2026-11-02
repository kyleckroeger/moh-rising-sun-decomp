// Fragment of capsule.cpp: the plane test, which tests a sphere at the nearer end when the
// line stays on one side.
#include "Capsule.h"
#include "Sphere.h"
#include "Frustum.h"
#include <math.h>

static inline float PlaneDist(const CPlane &plane, const CVector3 &point) {
    float sum = point.x * plane.normal.x + point.y * plane.normal.y;
    sum += point.z * plane.normal.z;
    return sum - plane.distance;
}

static inline CVector3 StartOf(const CLine3 &line) { return line.GetStart(); }
static inline CVector3 EndOf(const CLine3 &line) { return line.GetEnd(); }

EClsnState CVolCapsule::TestCollision(const CPlane &plane, CCollision &collision, bool flag) const {
    CVector3 start = StartOf(line);
    CVector3 end = EndOf(line);
    float b = PlaneDist(plane, end);
    // Plain reference: reproduces the code shape.
    const CVector3 &s = start;
    float a = PlaneDist(plane, s);
    if (!(a * b <= radius * radius)) {
        float absA = fabs(a);
        float absB = fabs(b);
        if (!(absA > absB)) {
            CVolSphere sphere(radius, start);
            return sphere.CVolSphere::TestCollision(plane, collision, flag);
        }
        CVolSphere sphere(radius, end);
        return sphere.CVolSphere::TestCollision(plane, collision, flag);
    }
    if (flag)
        collision.RaiseResult(2);
    return (EClsnState)2;
}
