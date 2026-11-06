// Fragment of cdbobject.cpp: the box, sphere, capsule and animated tests, which gather tri
// groups from the tree and test them.
#include "CDBObject.h"
#include "CDBGeom.h"
#include "Sphere.h"
#include "AnimatedVolume.h"
#include "Capsule.h"
#include <math.h>

extern bool g_bDisablePassThru;

// The sphere tests each group's plane itself rather than through the virtual call.
template <> inline EClsnState TestCollisionWithTriGroups(const dwi::fast_vec<CDBTriGroup *> &groups, const CVolSphere &sphere, CCollision &collision, bool flag) {
    int result = 1;
    CDBTriGroup **end = groups.end();
    CDBTriGroup **it = groups.begin();
    if (it != end) {
        CVector3 center(sphere.center);
        float radius = sphere.radius;
        for (; it != end; it++) {
            CDBTriGroup *group = *it;
            CVector3 normal(group->normal.x, group->normal.y, group->normal.z);
            float dot = normal.x * center.x + normal.y * center.y;
            dot += normal.z * center.z;
            float dist = fabs(dot - group->distance);
            if (!(dist > radius)) {
                int count = group->mark;
                CDBTri **tris = group->tris;
                if (count != 0) {
                    count = -count;
                    do {
                        CDBTri *tri = *tris;
                        if (g_bDisablePassThru && CSurfaceType::IsPassThru(tri->surface, tri->flags >= 0))
                            break;
                        CCDBTriangle triangle(*tri);
                        int r = triangle.TestCollision(sphere, collision, tri, flag);
                        if (r > result)
                            result = r;
                        tris++;
                    } while (--count != 0);
                }
            }
        }
    }
    return (EClsnState)result;
}

EClsnState CCDBObject::TestCollision(const CVolBox &other, CCollision &collision, bool flag) const {
    CDB *db = database;
    if (!db)
        return (EClsnState)1;
    CDBTriGroup *storage[2048];
    dwi::fast_vec<CDBTriGroup *> groups(storage, 2048);
    EClsnState result = (EClsnState)1;
    EClsnState node;
    if (db->branchCount > 0)
        node = TestCDBBranchNodeCollision(*db->branchRoot, other, collision, flag, groups);
    else
        node = TestCDBLeafNodeCollision(*db->leafRoot, other, collision, flag, groups);
    if (node != 1)
        result = TestCollisionWithTriGroups(groups, other, collision, flag);
    CDBTriGroup **end = groups.end();
    for (CDBTriGroup **group = groups.begin(); group != end; group++)
        (*group)->mark = -(*group)->mark;
    return result;
}

EClsnState CCDBObject::TestCollision(const CVolSphere &other, CCollision &collision, bool flag) const {
    CDB *db = database;
    if (!db)
        return (EClsnState)1;
    CDBTriGroup *storage[2048];
    dwi::fast_vec<CDBTriGroup *> groups(storage, 2048);
    EClsnState result = (EClsnState)1;
    EClsnState node;
    if (db->branchCount > 0)
        node = TestCDBBranchNodeCollision(*db->branchRoot, other, collision, flag, groups);
    else
        node = TestCDBLeafNodeCollision(*db->leafRoot, other, collision, flag, groups);
    if (node != 1)
        result = TestCollisionWithTriGroups(groups, other, collision, flag);
    CDBTriGroup **end = groups.end();
    for (CDBTriGroup **group = groups.begin(); group != end; group++)
        (*group)->mark = -(*group)->mark;
    return result;
}

EClsnState CCDBObject::TestCollision(const CVolCapsule &other, CCollision &collision, bool flag) const {
    CDB *db = database;
    if (!db)
        return (EClsnState)1;
    CDBTriGroup *storage[2048];
    dwi::fast_vec<CDBTriGroup *> groups(storage, 2048);
    EClsnState result = (EClsnState)1;
    EClsnState node;
    if (db->branchCount > 0)
        node = TestCDBBranchNodeCollision(*db->branchRoot, other, collision, flag, groups);
    else
        node = TestCDBLeafNodeCollision(*db->leafRoot, other, collision, flag, groups);
    if (node != 1)
        result = TestCollisionWithTriGroups(groups, other, collision, flag);
    CDBTriGroup **end = groups.end();
    for (CDBTriGroup **group = groups.begin(); group != end; group++)
        (*group)->mark = -(*group)->mark;
    return result;
}

EClsnState CCDBObject::TestCollision(const CAnimatedVolume &other, CCollision &collision, bool flag) const {
    CDB *db = database;
    if (!db)
        return (EClsnState)1;
    CDBTriGroup *storage[2048];
    dwi::fast_vec<CDBTriGroup *> groups(storage, 2048);
    EClsnState result = (EClsnState)1;
    EClsnState node;
    if (db->branchCount > 0)
        node = TestCDBBranchNodeCollision(*db->branchRoot, other, collision, flag, groups);
    else
        node = TestCDBLeafNodeCollision(*db->leafRoot, other, collision, flag, groups);
    if (node != 1)
        result = TestCollisionWithTriGroups(groups, other, collision, flag);
    CDBTriGroup **end = groups.end();
    for (CDBTriGroup **group = groups.begin(); group != end; group++)
        (*group)->mark = -(*group)->mark;
    return result;
}

