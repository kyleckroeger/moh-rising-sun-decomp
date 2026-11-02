// Fragment of fiber.cpp: the plane test.
#include "Fiber.h"
#include "Frustum.h"
#include "Capsule.h"

EClsnState CVolFiber::TestCollision(const CPlane &plane, CCollision &collision, bool flag) const {
    collision.lineTest = 1;
    float t;
    CVector3 point;
    int hit = plane.GetIntersection(line, t, &point);
    int inRange = 0;
    if (!(t < 0.0f))
        inRange = t <= 1.0f;
    hit &= inRange;
    bool inside = hit;
    if (!inside) {
        if (flag)
            collision.RaiseResult(1);
        return (EClsnState)1;
    }
    collision.depth = t;
    int result = 2;
    if (flag) {
        collision.RaiseResult(result);
        SClsnContact contact;
        contact.separation = 0.0f;
        contact.normal.x = -plane.normal.x;
        contact.normal.y = -plane.normal.y;
        contact.normal.z = -plane.normal.z;
        contact.position = point;
        contact.feature = 0;
        collision.AddContact(contact);
    }
    return (EClsnState)result;
}
