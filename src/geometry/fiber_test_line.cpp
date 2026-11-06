// Fragment of fiber.cpp: two line segments touch when their closest points coincide.
#include "Fiber.h"
#include <math.h>

static inline CVector3 StartCopy(const CLine3 &line) { return CVector3(line.start); }
static inline CVector3 EndCopy(const CLine3 &line) { return CVector3(line.end); }
static inline CVector3 DirectionCopy(const CLine3 &line) { return CVector3(line.direction); }
static inline void SetSub(CVector3 &r, const CVector3 &a, const CVector3 &b) {
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
}
static inline void SetScaled(CVector3 &r, const CVector3 &a, float s) {
    r.x = a.x * s;
    r.y = a.y * s;
    r.z = a.z * s;
}
static inline float MagnitudeZ(const CVector3 &v) { return fabs(v.z); }
static inline float DotOf(const CVector3 &a, const CVector3 &b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

EClsnState CVolFiber::TestCollision(const CLine3 &other, CCollision &collision, bool flag) const {
    collision.lineTest = 1;
    CVector3 p21;
    CVector3 p43;
    CVector3 p13;
    SetSub(p21, EndCopy(line), StartCopy(line));
    SetSub(p43, EndCopy(other), StartCopy(other));
    SetSub(p13, StartCopy(line), StartCopy(other));
    if (!((float)fabs(p21.x) >= 1e-12f) && !((float)fabs(p21.y) >= 1e-12f) && MagnitudeZ(p21) < 1e-12f) {
        if (flag)
            collision.RaiseResult(1);
        return (EClsnState)1;
    }
    if (!((float)fabs(p43.x) >= 1e-12f) && !((float)fabs(p43.y) >= 1e-12f) && (float)fabs(p43.z) < 1e-12f) {
        if (flag)
            collision.RaiseResult(1);
        return (EClsnState)1;
    }
    float d4321 = DotOf(p43, p21);
    float d4343 = p43.LengthSquared();
    float d2121 = p21.LengthSquared();
    float denom = d2121 * d4343 - d4321 * d4321;
    if ((float)fabs(denom) < 1e-12f) {
        if (flag)
            collision.RaiseResult(1);
        return (EClsnState)1;
    }
    float inv = 1.0f / denom;
    float d1321 = DotOf(p13, p21);
    float d1343 = DotOf(p13, p43);
    float mua = (d1343 * d4321 - d1321 * d4343) * inv;
    if (mua < 0.0f || mua > 1.0f) {
        if (flag)
            collision.RaiseResult(1);
        return (EClsnState)1;
    }
    float mub = (d4321 * mua + d1343) * inv;
    if (mub < 0.0f || mub > 1.0f) {
        if (flag)
            collision.RaiseResult(1);
        return (EClsnState)1;
    }
    CVector3 pa;
    CVector3 pb;
    SetScaled(pa, p21, mua);
    pa += StartCopy(line);
    SetScaled(pb, p43, mub);
    pb += StartCopy(other);
    pa -= pb;
    if ((float)fabs(pa.x) > 0.001f || (float)fabs(pa.y) > 0.001f || !((float)fabs(pa.z) <= 0.001f)) {
        if (flag)
            collision.RaiseResult(1);
        return (EClsnState)1;
    }
    if (flag) {
        collision.RaiseResult(2);
        SClsnContact contact;
        CVector3 n;
        CVector3 l;
        SetSub(n, pa, StartCopy(line));
        contact.separation = sqrtf(n.LengthSquared());
        collision.depth = mua;
        n = DirectionCopy(line);
        l = DirectionCopy(other);
        contact.normal.x = n.y * l.z - n.z * l.y;
        contact.normal.y = n.z * l.x - n.x * l.z;
        contact.normal.z = n.x * l.y - n.y * l.x;
        contact.position = pa;
        contact.feature = 0;
        collision.AddContact(contact);
    }
    return (EClsnState)2;
}
