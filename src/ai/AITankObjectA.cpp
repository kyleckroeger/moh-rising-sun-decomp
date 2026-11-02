// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original AITankObject.cpp; status-variable meanings and member
// names are descriptive. Calls to CAIObject and its table convert the pointer unadjusted.
#include "AIBehavior.h"
#include "AIFilterGlobal.h"
#include "AIDoodad.h"

void *CAITankObject::operator new(unsigned int, void *place) { return place; }

void CAITankObject::operator delete(void *) {}

void CAITankObject::UpdateFromGameObject(IMovingSceneNode *node) {
    ((CAIObject *)this)->UpdateFromGameObject(node);
    health = *(float *)((char *)((CAIObject *)this)->filter->bsgo + 20);
}

void CAITankObject::PreUpdate() {
    CAIObjectInterface *self = (CAIObjectInterface *)this;
    self->UpdateFromGameObject((IMovingSceneNode *)self->GetSceneNode());
    ((CAIObject *)this)->PreUpdate();
}
