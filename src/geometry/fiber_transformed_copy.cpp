// Fragment of fiber.cpp: the line's ends are transformed and the line set from them.
#include "Fiber.h"
#include "CMatrix.h"

static inline CVector3 StartCopy(const CLine3 &line) { return CVector3(line.start); }
static inline CVector3 EndCopy(const CLine3 &line) { return CVector3(line.end); }
static inline void TransformTo(CVector3 &out, const CMatrix &m, CVector3 p) {
    CVector3 r;
    r.x = p.x * m.row[0].x + p.y * m.row[1].x + p.z * m.row[2].x + m.row[3].x;
    r.y = p.x * m.row[0].y + p.y * m.row[1].y + p.z * m.row[2].y + m.row[3].y;
    r.z = p.x * m.row[0].z + p.y * m.row[1].z + p.z * m.row[2].z + m.row[3].z;
    out = r;
}
static inline void SubtractComponents(CVector3 &out, const CVector3 &a, const CVector3 &b) {
    out.x = a.x - b.x;
    out.y = a.y - b.y;
    out.z = a.z - b.z;
}
static inline void SetLine(CLine3 &l, const CVector3 &a, const CVector3 &b) {
    l.start = a;
    l.end = b;
    SubtractComponents(l.direction, l.end, l.start);
    l.lengthValid = 0;
    l.lengthSquaredValid = 0;
}

void CVolFiber::TransformedCopy(const IVolume &volume, const CMatrix &m) {
    const CVolFiber &other = (const CVolFiber &)volume;
    line = other.line;
    CVector3 a;
    CVector3 b;
    TransformTo(a, m, StartCopy(other.line));
    TransformTo(b, m, EndCopy(other.line));
    SetLine(line, a, b);
}
