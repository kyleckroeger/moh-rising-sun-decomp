// Fragment of plane.cpp: transforming a plane, and where a line crosses it.
#include "Frustum.h"
#include "CMatrix.h"
#include "CLine3.h"
#include <math.h>

static inline CVector3 StartCopy(const CLine3 &line) { return CVector3(line.start); }
static inline CVector3 DirectionCopy(const CLine3 &line) { return CVector3(line.direction); }
static inline void SetScaled(CVector3 &r, const CVector3 &a, float s) {
    r.x = a.x * s;
    r.y = a.y * s;
    r.z = a.z * s;
}
static inline void TransformPoint(CVector3 &out, const CMatrix &m, const CVector3 &p) {
    CVector3 r;
    r.x = p.x * m.row[0].x + p.y * m.row[1].x + p.z * m.row[2].x + m.row[3].x;
    r.y = p.x * m.row[0].y + p.y * m.row[1].y + p.z * m.row[2].y + m.row[3].y;
    r.z = p.x * m.row[0].z + p.y * m.row[1].z + p.z * m.row[2].z + m.row[3].z;
    out = r;
}
static inline void TransformVector(CVector3 &out, const CMatrix &m, CVector3 p) {
    CVector3 r;
    r.x = p.x * m.row[0].x + p.y * m.row[1].x + p.z * m.row[2].x;
    r.y = p.x * m.row[0].y + p.y * m.row[1].y + p.z * m.row[2].y;
    r.z = p.x * m.row[0].z + p.y * m.row[1].z + p.z * m.row[2].z;
    out = r;
}

void CPlane::Transform(const CMatrix &m) {
    CVector3 scaled;
    SetScaled(scaled, normal, distance);
    CVector3 point;
    TransformPoint(point, m, scaled);
    TransformVector(normal, m, normal);
    distance = normal.x * point.x + normal.y * point.y + normal.z * point.z;
}

int CPlane::GetIntersection(const CLine3 &line, float &t, CVector3 *point) const {
    CVector3 dir;
    dir = DirectionCopy(line);
    // Plain references: reproduce the code shape.
    const CVector3 &dd = dir;
    const CVector3 &nn = normal;
    float denominator = nn.x * dd.x + nn.y * dd.y;
    denominator += nn.z * dd.z;
    float numerator;
    {
        CVector3 sc(line.start);
        numerator = normal.x * sc.x + normal.y * sc.y;
        numerator += normal.z * sc.z;
    }
    float absolute = fabs(denominator);
    if (!(absolute <= line.CachedLength() * 0.00174532796f)) {
        t = (distance - numerator) / denominator;
    } else {
        float limit = sqrtf(StartCopy(line).LengthSquared()) * 0.00174532796f;
        if (!((float)fabs(numerator) <= limit))
            t = -1.0f;
        else
            t = 1.0f;
    }
    int hit = 0;
    if (!(t < 0.0f))
        hit = t <= 1.0f;
    if (hit && point) {
        SetScaled(*point, dir, t);
        *point += StartCopy(line);
    }
    return hit;
}
