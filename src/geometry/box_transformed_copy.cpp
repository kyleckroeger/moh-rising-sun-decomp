// Fragment of box.cpp: TransformedCopy transforms the axes and centre, and SetCorners
// measures half sizes along the axes; both normalize the axes.
#include "Box.h"
#include "CMatrix.h"
extern "C" float sqrtf(float);

static inline void TransformDirection(CVector3 &out, const CMatrix &m, const CVector3 &p) {
    CVector3 r;
    r.x = p.x * m.row[0].x + p.y * m.row[1].x + p.z * m.row[2].x;
    r.y = p.x * m.row[0].y + p.y * m.row[1].y + p.z * m.row[2].y;
    r.z = p.x * m.row[0].z + p.y * m.row[1].z + p.z * m.row[2].z;
    out = r;
}
static inline void TransformPosition(CVector3 &out, const CMatrix &m, const CVector3 &p) {
    CVector3 r;
    r.x = p.x * m.row[0].x + p.y * m.row[1].x + p.z * m.row[2].x + m.row[3].x;
    r.y = p.x * m.row[0].y + p.y * m.row[1].y + p.z * m.row[2].y + m.row[3].y;
    r.z = p.x * m.row[0].z + p.y * m.row[1].z + p.z * m.row[2].z + m.row[3].z;
    out = r;
}

static inline void Normalize(CVector3 &v) {
    float length = sqrtf(v.LengthSquared());
    if (length != 0.0f)
        v *= 1.0f / length;
}
static inline void SetSum(CVector3 &r, const CVector3 &a, const CVector3 &b) {
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    r.z = a.z + b.z;
}
static inline void SetSub(CVector3 &r, const CVector3 &a, const CVector3 &b) {
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
}
void CVolBox::TransformedCopy(const IVolume &volume, const CMatrix &m) {
    const CVolBox &other = (const CVolBox &)volume;
    halfSize[0] = other.halfSize[0];
    halfSize[1] = other.halfSize[1];
    halfSize[2] = other.halfSize[2];
    TransformDirection(axisX, m, other.axisX);
    TransformDirection(axisY, m, other.axisY);
    TransformDirection(axisZ, m, other.axisZ);
    TransformPosition(center, m, other.center);
    Normalize(axisX);
    Normalize(axisY);
    Normalize(axisZ);
}

static inline float DotOf(const CVector3 &a, const CVector3 &b) {
    float sum = a.x * b.x + a.y * b.y;
    sum += a.z * b.z;
    return sum;
}

void CVolBox::SetCorners(const CVector3 &a, const CVector3 &b) {
    SetSum(center, b, a);
    center *= 0.5f;
    CVector3 diff;
    CVector3 size;
SetSub(diff, center, a);
    size.Set(DotOf(axisX, diff), DotOf(axisY, diff), DotOf(axisZ, diff));
    halfSize[0] = size.x;
    halfSize[1] = size.y;
    halfSize[2] = size.z;
    Normalize(axisX);
    Normalize(axisY);
    Normalize(axisZ);
}
