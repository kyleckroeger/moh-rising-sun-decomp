// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original AISoldierObject.cpp; status-variable meanings and member
// names are descriptive. Calls to CAIObject and its table convert the pointer unadjusted.
#include "AIBehavior.h"
#include "AIFilterGlobal.h"
#include "AIDoodad.h"
#include "ScriptTimers.h"
#include "SceneObjects.h"

void CAISoldierObject::SetSoldierObject(CSoldierObject *soldier) {
    soldierObject = soldier;
    snap.soldierObject = soldier;
}

void CAISoldierObject::PreUpdate() {
    CAIObjectInterface *self = (CAIObjectInterface *)this;
    self->UpdateFromGameObject((IMovingSceneNode *)self->GetSceneNode());
    ((CAIObject *)this)->PreUpdate();
}

void CAISoldierObject::UpdateFromGameObject(IMovingSceneNode *node) {
    ((CAIObject *)this)->UpdateFromGameObject(node);
    health = *(float *)((char *)((CAIObject *)this)->filter->bsgo + 20);
}

void CAISoldierObject::Update(float dt) {
    CAIObject *self = (CAIObject *)this;
    self->Update(dt);
    self->targeting.Update();
    CAILocomotion *locomotion = &self->locomotion;
    switch (locomotion->walkType) {
    case 0:
        break;
    case 1:
        self->splinePath.WalkNextSplinePathPoint();
        break;
    case 2:
        self->areaPathFinding.WalkAStarPath();
        break;
    case 4:
        ((CCoverpointBehavior *)coverpointStorage)->WalkToCoverPoint();
        break;
    case 3:
        locomotion->WalkToArbitraryPoint();
        break;
    }
    UpdateStatus();
    snap.Update(dt);
    locomotion = &self->locomotion;
    if (locomotion->walkType)
        ((CAnimObjectInterface *)soldierObject)->SetMoveTarget(locomotion->GetCurrentObjectivePosition());
    else if (self->CheckIncomingFlagAll(2))
        ((CAnimObjectInterface *)soldierObject)->SetMoveTarget(self->targeting.modifiedPosition);
    if ((self->statusFlags2 & 1) && !(self->previousStatusFlags2 & 1))
        BSObjectTriggerEvent(self->filter->scriptObject, 125, 0, 0, false);
    threatEvaluation.Update(dt);
}

float CAISoldierObject::GetAIValueByName(const char *name) {
    return ((CCoverpointBehavior *)coverpointStorage)->GetAIValueByName(name);
}

void CAISoldierObject::Init() {
    ((CAIObject *)this)->Init();
    snap.Init();
    SetInitialStatus();
    targetMatchCount = 0;
}

void CAISoldierObject::SetInitialStatus() {
    CAIObject *self = (CAIObject *)this;
    self->SetAIStatusVar((aistatus_var)0, (int)(15.0f * (health / maxHealth) + 0.999));
    self->SetAIStatusVar((aistatus_var)1, self->unknown599);
    self->SetAIStatusVar((aistatus_var)2, 4);
    self->SetAIStatusVar((aistatus_var)3, 1);
    self->SetAIStatusVar((aistatus_var)4, self->team);
    self->SetAIStatusVar((aistatus_var)6, 0);
    self->SetAIStatusVar((aistatus_var)11, 0);
}

void CAISoldierObject::UpdateStatus() {
    CAIObject *self = (CAIObject *)this;
    self->SetAIStatusVar((aistatus_var)0, (int)(15.0f * (health / maxHealth) + 0.999));
    self->SetAIStatusVar((aistatus_var)2, 4);
    self->SetAIStatusVar((aistatus_var)3, 1);
}

void CAISoldierObject::SetupTargetMatchList(aistatus_match *list, int count) {
    targetMatchCount = count;
    aistatus_match *out = targetMatchList;
    for (; count > 0; count--)
        *out++ = *list++;
}

float ScoreTarget(CAIObject *object, CAIObject *other) {
    float distance = other->GetDistanceXYZReal(object);
    if (((CAIObjectInterface *)object)->IsPlayer() && !(distance >= 10.0f))
        distance *= 0.5f;
    return distance;
}

float ScoreTargetByDistance(const CVector3 &object, const CVector3 &other) {
    float distance = other.Distance(object);
    if (!(distance >= 10.0f))
        distance *= 0.5f;
    return distance;
}
