// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original Soldier_object.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "SceneNode.h"
#include "ObjCreate.h"

class CStaticObjectFactory {
  public:
    static CStaticObject *AllocateStaticObject(int, int, PickupInfo *);
};
namespace EALA {
namespace Character {
class Skeleton {
  public:
    const CMatrix *FindAttachPoint(const char *);
};
}
}
typedef CAttachableObject::AttachMesh::Node AttachNode;

int CSoldierObject::AttachObject(int crc, char *name, int nodeId) {
    const CMatrix *point = ((EALA::Character::Skeleton *)((CAnimObject *)this)->skeleton)->FindAttachPoint(name);
    int i = GetAttachPoint(name);
    if (attachSlots[i].active)
        return 0;
    CStaticObject *object = CStaticObjectFactory::AllocateStaticObject(crc, -1, 0);
    attachSlots[i].object = object;
    attachSlots[i].keepObject = 0;
    attachSlots[i].active = 1;
    attachSlots[i].owned = 1;
    attachSlots[i].addToScene = 0;
    CAttachableObject::AttachMesh **meshes = ((CAttachableObject *)object)->mesh;
    attachSlots[i].matrix = point;
    attachSlots[i].hasInverse = 0;
    if (nodeId) {
        CAttachableObject::AttachMesh *mesh = *meshes;
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

ISceneNode *CSoldierObject::GetAttachedObjectAt(char *name) {
    int i = GetAttachPoint(name);
    if (i == -1 || !attachSlots[i].active || attachSlots[i].keepObject)
        return 0;
    return (ISceneNode *)attachSlots[i].object;
}

int CSoldierObject::AttachObject(CStaticObject *object, char *name, int nodeId) {
    if (!object)
        return 0;
    int i = GetAttachPoint(name);
    if (i == -1 || attachSlots[i].active)
        return 0;
    attachSlots[i].object = object;
    attachSlots[i].active = 1;
    attachSlots[i].keepObject = 0;
    attachSlots[i].owned = 0;
    CAttachableObject::AttachMesh **meshes = ((CAttachableObject *)object)->mesh;
    const CMatrix *point = ((EALA::Character::Skeleton *)((CAnimObject *)this)->skeleton)->FindAttachPoint(name);
    if (point) {
        attachSlots[i].matrix = point;
        attachSlots[i].hasInverse = 0;
    } else {
        attachSlots[i].active = 0;
        attachSlots[i].object = 0;
        return 0;
    }
    if (nodeId) {
        CAttachableObject::AttachMesh *mesh = *meshes;
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
    if (g_scene.IsNodeInScene((ISceneNode *)object)) {
        attachSlots[i].addToScene = 1;
        g_scene.Remove(*(ISceneNode *)object);
    } else {
        attachSlots[i].addToScene = 0;
    }
    return 1;
}
