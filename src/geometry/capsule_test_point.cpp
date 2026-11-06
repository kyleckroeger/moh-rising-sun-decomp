// Fragment of capsule.cpp: a point is tested against the nearest point on the line.
#include "Capsule.h"
extern "C" float sqrtf(float);

// Descriptive helpers. The by-value copies and by-reference results reproduce the original's
// temporaries and its use of an address register for the nearest point.
static inline CVector3 StartCopy(const CLine3 &line) { return CVector3(line.start); }
static inline CVector3 EndCopy(const CLine3 &line) { return CVector3(line.end); }
static inline void SetSub(CVector3 &r, const CVector3 &a, const CVector3 &b) {
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
}
static inline void SetSum(CVector3 &r, const CVector3 &a, const CVector3 &b) {
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    r.z = a.z + b.z;
}
static inline void SetScaled(CVector3 &r, const CVector3 &a, float s) {
    r.x = a.x * s;
    r.y = a.y * s;
    r.z = a.z * s;
}

EClsnState CVolCapsule::TestCollision(const CVector3 &point, CCollision &collision, bool flag) const {
    CVector3 nearest;
    CVector3 direction;
    float t;
    line.GetProjection(point, nearest, t);
    if (!(t >= 0.0f)) {
        t = 0.0f;
        nearest = StartCopy(line);
    } else if (!(t <= 1.0f)) {
        t = 1.0f;
        nearest = EndCopy(line);
    }
    SetSub(direction, point, nearest);
    float separation = sqrtf(direction.LengthSquared()) - radius;
    int result = 2;
    if (!(separation < 0.0f))
        result = 1;
    if (flag) {
        collision.RaiseResult(result);
        if (result == 2) {
            CVector3 offset;
            float length = sqrtf(direction.LengthSquared());
            if (length != 0.0f)
                direction *= 1.0f / length;
            SetScaled(offset, direction, radius);
            SClsnContact contact;
            contact.separation = separation;
            SetSum(contact.position, nearest, offset);
            contact.normal = direction;
            contact.feature = 0;
            contact.subVolume = 0;
            collision.AddContact(contact);
        }
    }
    return (EClsnState)result;
}
