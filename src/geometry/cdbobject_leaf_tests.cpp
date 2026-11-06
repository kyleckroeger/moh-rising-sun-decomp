// Fragment of cdbobject.cpp: the leaf node tests, which gather a leaf's unmarked tri
// groups when the volume reaches its box.
#include "CDBObject.h"
#include "CDBGeom.h"
#include "Sphere.h"
#include "AnimatedVolume.h"
#include "Capsule.h"
#include <math.h>

// Classifies a volume against a node's plane: the distance beyond its extent, or 0 when
// the plane crosses it.
typedef dwi::fast_vec<CDBTriGroup *> GroupList;

// The node tests are overloads in the original's symbols; the shared part gathers a
// reached leaf's unmarked groups.
static inline EClsnState GatherGroups(const CDBLeafNode &node, EClsnState result, GroupList &groups) {
    if (result != 1) {
        for (int i = 0; i < node.groupCount; i++) {
            CDBTriGroup *group = node.groups[i];
            if (group->mark > 0) {
                group->mark = -group->mark;
                groups.push_back(group);
            }
        }
    }
    return result;
}

// The original calls the base class's test here, not CCDBVolBox's override.
EClsnState CCDBObject::TestCDBLeafNodeCollision(const CDBLeafNode &node, const CVolBox &volume, CCollision &collision, bool, GroupList &groups) const {
    CCDBVolBox box(node.box);
    return GatherGroups(node, box.CVolBox::TestCollision(volume, collision, false), groups);
}

EClsnState CCDBObject::TestCDBLeafNodeCollision(const CDBLeafNode &node, const CVolSphere &volume, CCollision &collision, bool, GroupList &groups) const {
    CCDBVolBox box(node.box);
    return GatherGroups(node, box.TestCollision(volume, collision, false), groups);
}

EClsnState CCDBObject::TestCDBLeafNodeCollision(const CDBLeafNode &node, const CVolCapsule &volume, CCollision &collision, bool, GroupList &groups) const {
    CCDBVolBox box(node.box);
    return GatherGroups(node, box.TestCollision(volume, collision, false), groups);
}

EClsnState CCDBObject::TestCDBLeafNodeCollision(const CDBLeafNode &node, const CAnimatedVolume &volume, CCollision &collision, bool, GroupList &groups) const {
    CCDBVolBox box(node.box);
    return GatherGroups(node, box.TestCollision(volume, collision, false), groups);
}
