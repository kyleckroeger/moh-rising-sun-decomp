// The box, capsule and animated instances of TestCollisionWithTriGroups, in the linker's
// weak-code region; their literals follow cdbobject.cpp's.
#include "CDBGeom.h"
#include "Capsule.h"
#include "AnimatedVolume.h"
#include "Sphere.h"
#include "Frustum.h"

extern bool g_bDisablePassThru;

template <class T>
EClsnState TestCollisionWithTriGroups(const dwi::fast_vec<CDBTriGroup *> &groups, const T &volume, CCollision &collision, bool flag) {
    int result = 1;
    CDBTriGroup **end = groups.end();
    for (CDBTriGroup **it = groups.begin(); it != end; it++) {
        CDBTriGroup *group = *it;
        CVector3 normal(group->normal.x, group->normal.y, group->normal.z);
        CPlane plane(normal, group->distance);
        if (volume.TestCollision(plane, collision, false) != 1) {
            CDBTri **tris = group->tris;
            for (int i = 0; i < -group->mark; i++, tris++) {
                CDBTri *tri = *tris;
                if (g_bDisablePassThru && CSurfaceType::IsPassThru(tri->surface, tri->flags >= 0))
                    break;
                CCDBTriangle triangle(*tri);
                int r = triangle.TestCollision(volume, collision, tri, flag);
                if (r > result)
                    result = r;
            }
        }
    }
    return (EClsnState)result;
}

template EClsnState TestCollisionWithTriGroups<CVolBox>(const dwi::fast_vec<CDBTriGroup *> &, const CVolBox &, CCollision &, bool);
template EClsnState TestCollisionWithTriGroups<CVolCapsule>(const dwi::fast_vec<CDBTriGroup *> &, const CVolCapsule &, CCollision &, bool);
template EClsnState TestCollisionWithTriGroups<CAnimatedVolume>(const dwi::fast_vec<CDBTriGroup *> &, const CAnimatedVolume &, CCollision &, bool);
