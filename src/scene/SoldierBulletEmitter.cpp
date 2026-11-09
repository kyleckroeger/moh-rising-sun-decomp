// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original Soldier_object.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "Weapon.h"
typedef CAttachableObject::AttachMesh AttachMeshData;
typedef AttachMeshData::Node AttachNode;

static inline void PostMultiply(CMatrix &m, const CMatrix &o) {
    CMatrix::s_TempMat = m;
    m.Multiply(o, CMatrix::s_TempMat);
}

void CSoldierObject::SetBulletEmitter(CStaticObject *object, char *name) {
    int flash = (int)weapons[currentWeaponSlot]->muzzleFlash;
    if (!flash) {
        emitterActive = flash;
        return;
    }
    int i = GetAttachPoint(name);
    if (i < 0 || !attachSlots[i].active)
        return;
    AttachMeshData *mesh = *((CAttachableObject *)object)->mesh;
    int j = 0;
    int emitterNode = 0xe6e804d5;
    int count = mesh->nodeCount;
    if (count > 0) {
        AttachNode *node = mesh->nodes;
        for (; j < count; j++, node++) {
            if (node->id == emitterNode) {
                emitterSlot = i;
                emitterMatrix.GetSlot(0);
                if (attachSlots[i].hasInverse)
                    PostMultiply(emitterMatrix, attachSlots[i].inverse);
                CMatrix::s_TempMat = emitterMatrix;
                emitterMatrix.Multiply(node->matrix, CMatrix::s_TempMat);
                emitterActive = 1;
                break;
            }
        }
        if (j >= count) {
            emitterSlot = 0;
            emitterActive = 0;
        }
    }
}
