// Fragment of sphere.cpp: the plane test. A contact's position is the center moved along
// the plane normal by the penetration depth.
#include "Sphere.h"
#include "Frustum.h"


static inline float PlaneDistance(const CPlane &plane, const CVector3 &point) {
    float sum = plane.normal.x * point.x + plane.normal.y * point.y;
    sum += plane.normal.z * point.z;
    return sum - plane.distance;
}

EClsnState CVolSphere::TestCollision(const CPlane &plane, CCollision &collision, bool flag) const {
    float separation = PlaneDistance(plane, center) - radius;
    int result = 2;
    if (!(separation < 0.0f))
        result = 1;
    if (flag) {
        collision.RaiseResult(result);
        if (result == 2) {
            float depth = -separation;
            SClsnContact contact;
            contact.position.x = plane.normal.x * depth;
            contact.position.y = plane.normal.y * depth;
            contact.position.z = plane.normal.z * depth;
            contact.position += center;
            CVector3 &normal = contact.normal;
            normal.x = -plane.normal.x;
            normal.y = -plane.normal.y;
            normal.z = -plane.normal.z;
            contact.feature = 0;
            collision.AddContact(contact);
        }
    }
    return (EClsnState)result;
}

