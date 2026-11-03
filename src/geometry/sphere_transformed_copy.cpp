// Fragment of sphere.cpp: the centre is transformed and the radius scaled by the matrix's
// first row.
#include "Sphere.h"
#include "CMatrix.h"
extern "C" float sqrtf(float);

static inline void TransformTo(CVector3 &out, const CMatrix &m, const CVector3 &p) {
    CVector3 r;
    r.x = p.x * m.row[0].x + p.y * m.row[1].x + p.z * m.row[2].x + m.row[3].x;
    r.y = p.x * m.row[0].y + p.y * m.row[1].y + p.z * m.row[2].y + m.row[3].y;
    r.z = p.x * m.row[0].z + p.y * m.row[1].z + p.z * m.row[2].z + m.row[3].z;
    out = r;
}

void CVolSphere::TransformedCopy(const IVolume &volume, const CMatrix &m) {
    const CVolSphere &other = (const CVolSphere &)volume;
    TransformTo(center, m, other.center);
    radius = sqrtf(((const CVector3 &)m.row[0]).LengthSquared()) * other.radius;
}
