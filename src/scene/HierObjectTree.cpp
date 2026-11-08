// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original hierobject.cpp; member names are descriptive.
#include "SceneObjects.h"

void CHierObject::SetRotation(CQuaternion &rotation) { orientation = rotation; }
void CHierObject::GetRotation(CQuaternion &rotation) { rotation = orientation; }
CStaticObject *CHierObject::GetSubObject(int index) { return subObjects[index]; }

void CHierObject::GetAttachPointMatrix(int index, CMatrix &matrix) {
    ((CStaticObjectInterface *)this)->FindAttachNode(index, &matrix);
}

void CHierObject::AddObjectToTree(CHierObject *object, int index, int parent) {
    subObjects[index] = object;
    subObjects[parent]->AddHierChild(object);
}

int CHierObject::RemoveObjectFromTree(CHierObject *object, int index) {
    if (hierFlags.root)
        subObjects[index] = 0;
    if (object == firstChild) {
        firstChild = object->nextSibling;
        return 1;
    }
    CHierObject *previous = 0;
    for (CHierObject *child = firstChild; child; child = child->nextSibling) {
        if (child == object) {
            previous->nextSibling = child->nextSibling;
            return 1;
        }
        if (child->RemoveObjectFromTree(object, index))
            return 1;
        previous = child;
    }
    return 0;
}

extern void FreeBoxVolume(void *);
class CStaticObjectFactory {
  public:
    static void DeallocateStaticObject(CStaticObject *);
};

void CHierObject::CleanUp() {
    CHierObject *child = firstChild;
    while (child) {
        CHierObject *next = child->nextSibling;
        ((CStaticObjectInterface *)child)->CleanUp();
        child = next;
    }
    if (hierFlags.root)
        subObjects[0] = 0;
    else
        root->RemoveObjectFromTree(this, treeIndex);
    IVolume *local = (IVolume *)field_13c;
    if (local && worldVolume && local == worldVolumeType3 && local != worldVolume) {
        FreeBoxVolume(local);
        FreeBoxVolume(worldVolume);
    }
    CStaticObjectFactory::DeallocateStaticObject(this);
}

void CHierObject::AddHierChild(CHierObject *child) {
    if (firstChild)
        child->nextSibling = firstChild;
    firstChild = child;
}

void CHierObject::BeginUpdate(float dt) {
    if (!hierFlags.disabled)
        DoBeginUpdate(dt, 1);
}
