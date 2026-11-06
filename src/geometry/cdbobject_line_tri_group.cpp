// Fragment of cdbobject.cpp: tests a line against a tri group, keeping the nearest
// contact.
#include "CDBGeom.h"
#include "CLine3.h"
#include "Frustum.h"

extern bool g_bDisablePassThru;

static inline float PlaneDist(const CPlane &plane, const CVector3 &point) {
    float sum = plane.normal.x * point.x + plane.normal.y * point.y;
    sum += plane.normal.z * point.z;
    return sum - plane.distance;
}

EClsnState TestCollisionWithTriGroup(const CDBTriGroup &group, const CLine3 &line, CCollision &collision, bool flag) {
    int result = 1;
    CVector3 normal(group.normal.x, group.normal.y, group.normal.z);
    CPlane plane(normal, group.distance);
    float a = PlaneDist(plane, line.GetStart());
    float b = PlaneDist(plane, line.GetEnd());
    if (!(a * b >= 0.0f)) {
        CDBTri *const *tris = group.tris;
        for (int i = 0; i < -group.mark; i++, tris++) {
            CDBTri *tri = *tris;
            if (g_bDisablePassThru && CSurfaceType::IsPassThru(tri->surface, false))
                break;
            CCDBTriangle triangle(*tri);
            float previous = collision.depth;
            int r = triangle.TestCollision(line, collision, tri, flag);
            if (r != 1) {
                if (!(collision.depth > previous)) {
                    result = r;
                } else {
                    SClsnContact *contact = collision.contacts;
                    collision.depth = previous;
                    if (contact) {
                        SClsnContact *next = contact->next;
                        CCollision::ms_contactPool.Free(contact);
                        collision.contacts = next;
                        if (--collision.contactCount == 0)
                            collision.result = 0;
                    }
                }
            }
        }
    }
    return (EClsnState)result;
}
