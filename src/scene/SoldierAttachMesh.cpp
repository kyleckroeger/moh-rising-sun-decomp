// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original Soldier_object.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "SceneNode.h"
#include "ObjCreate.h"

namespace EALA {
namespace Character {
class Skeleton {
  public:
    const CMatrix *FindAttachPoint(const char *);
};
}
}
typedef CAttachableObject::AttachMesh AttachMeshData;
typedef AttachMeshData::Node AttachNode;

int CSoldierObject::AttachMesh(CStaticMesh *staticMesh, char *name, int nodeId) {
    if (!staticMesh)
        return 0;
    int i = GetAttachPoint(name);
    if (i == -1 || attachSlots[i].active)
        return 0;
    attachSlots[i].object = (CStaticObject *)staticMesh;
    attachSlots[i].owned = 0;
    attachSlots[i].keepObject = 1;
    attachSlots[i].active = 1;
    const CMatrix *point = ((EALA::Character::Skeleton *)((CAnimObject *)this)->skeleton)->FindAttachPoint(name);
    if (point) {
        attachSlots[i].matrix = point;
        attachSlots[i].hasInverse = 0;
    } else {
        attachSlots[i].matrix = 0;
        attachSlots[i].active = 0;
        attachSlots[i].object = 0;
        return 0;
    }
    if (nodeId) {
        AttachMeshData *mesh = *(AttachMeshData **)staticMesh;
        int count = mesh->nodeCount;
        AttachNode *node = mesh->nodes;
        for (int j = 0; j < count; j++, node++) {
            if (node->id == nodeId) {
                attachSlots[i].inverse.Inverse(node->matrix);
                attachSlots[i].hasInverse = 1;
                break;
            }
        }
    }
    return 1;
}

CStaticMesh *CSoldierObject::DetachMesh(char *name) {
    int i = GetAttachPoint(name);
    if (i < 0)
        return 0;
    CStaticMesh *mesh = (CStaticMesh *)attachSlots[i].object;
    attachSlots[i].object = 0;
    attachSlots[i].active = 0;
    attachSlots[i].owned = 0;
    return mesh;
}
