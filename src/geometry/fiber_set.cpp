// Fragment of fiber.cpp: Set inlines the line's Set.
#include "Fiber.h"

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
// The original inlines CLine3::Set here.
void CVolFiber::Set(const CVector3 &a, const CVector3 &b) { SetLine(line, a, b); }
