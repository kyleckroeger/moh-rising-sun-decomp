// Fragment of sphere.cpp: a point is inside when it is nearer the centre than the radius.
#include "Sphere.h"
extern "C" float sqrtf(float);

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

EClsnState CVolSphere::TestCollision(const CVector3 &point, CCollision &collision, bool flag) const {
    CVector3 direction;
    SetSub(direction, point, center);
    float distance = sqrtf(direction.LengthSquared());
    if (distance != 0.0f)
        direction *= 1.0f / distance;
    float separation = distance - radius;
    int result = 2;
    if (!(separation < 0.0f))
        result = 1;
    if (flag) {
        collision.RaiseResult(result);
        if (result == 2) {
            SClsnContact contact;
            contact.separation = separation;
            contact.normal = direction;
            direction *= radius;
            SetSum(contact.position, center, direction);
            contact.feature = 0;
            collision.AddContact(contact);
        }
    }
    return (EClsnState)result;
}
