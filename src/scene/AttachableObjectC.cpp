// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original attachobject.cpp; member names are descriptive.
#include "SceneObjects.h"
void CAttachableObject::BeginUpdate(float dt) { ((CStaticObject *)this)->BeginUpdate(dt); }
void CAttachableObject::UpdateAI(float) {}
void CAttachableObject::CommitAI() {}
void CAttachableObject::AttemptUpdate(float dt) {
    *(CMatrix *)((char *)this + 2368) = *(CMatrix *)((char *)this + 80);
    ((CStaticObject *)this)->AttemptUpdate(dt);
    UpdateAttachedObjects();
}
void CAttachableObject::CommitUpdate() { ((CStaticObject *)this)->CommitUpdate(); }
int CAttachableObject::FindAttachNode(int id, CMatrix *out) {
    AttachMesh *record = *mesh;
    int count = record->nodeCount;
    AttachMesh::Node *node = record->nodes;
    for (int i = 0; i < count; i++, node++) {
        if (node->id == id) {
            *out = node->matrix;
            return 1;
        }
    }
    return 0;
}
int CAttachableObject::GetObjectType() const { return 1; }
