// Fragment of fiber.cpp: a point lies on the fiber when its distance from the start
// matches its projection on the direction.
#include "Fiber.h"
#include <math.h>
static inline CVector3 StartCopy(const CLine3 &line) { return CVector3(line.start); }
static inline CVector3 DirectionCopy(const CLine3 &line) { return CVector3(line.direction); }
static inline void SetSub(CVector3 &r, const CVector3 &a, const CVector3 &b) {
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
}
static inline float DotOf(const CVector3 &a, const CVector3 &b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
static inline CVector3 CrossOf(const CVector3 &a, const CVector3 &b) {
    return CVector3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

static inline float MagnitudeZ(const CVector3 &v) { return fabs(v.z); }
static inline void SetCross(CVector3 &r, const CVector3 &a, const CVector3 &b) { CVector3 c; c.x = a.y * b.z - a.z * b.y; c.y = a.z * b.x - a.x * b.z; c.z = a.x * b.y - a.y * b.x; r.x=c.x;r.y=c.y;r.z=c.z; }

EClsnState CVolFiber::TestCollision(const CVector3 &point, CCollision &collision, bool flag) const {
    collision.lineTest = 1;
    CVector3 direction;
    CVector3 toPoint;
    direction = DirectionCopy(line);
    SetSub(toPoint, point, StartCopy(line));
    float projection = DotOf(direction, toPoint);
    float distance = sqrtf(toPoint.LengthSquared());
    if (projection < distance - 1e-12f || projection > distance + 1e-12f) {
        if (flag)
            collision.RaiseResult(1);
        return (EClsnState)1;
    }
    float length = line.CachedLength();
    if (projection < 0.0f || !(projection <= length)) {
        if (flag)
            collision.RaiseResult(1);
        return (EClsnState)1;
    }
    if (flag) {
        collision.RaiseResult(2);
        SClsnContact contact;
        if (!(length <= 1e-12f))
            collision.depth = projection / length;
        else
            collision.depth = 0.0f;
        contact.separation = projection;
        float absX = fabs(direction.x);
        float absY = fabs(direction.y);
        if (!(absX >= absY))
            SetCross(contact.normal, direction, CVector3(1.0f, 0.0f, 0.0f));
        else if (!(absY >= MagnitudeZ(direction)))
            SetCross(contact.normal, direction, CVector3(0.0f, 1.0f, 0.0f));
        else
            SetCross(contact.normal, direction, CVector3(0.0f, 0.0f, 1.0f));
        contact.position = point;
        contact.feature = 0;
        collision.AddContact(contact);
    }
    return (EClsnState)2;
}
