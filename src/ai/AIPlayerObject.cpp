// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original AIPlayerObject.cpp; the status-variable meanings and
// member names are descriptive. Calls to CAIObject convert the pointer unadjusted.
#include "AIDoodad.h"
#include "AIFilterGlobal.h"
#include "CameraShake.h"
extern int g_bCoopMultiplayer;
extern int g_bInMultiplayerMode;
#include "ScriptTimers.h"

void CAIPlayerObject::UpdateFromGameObject(IMovingSceneNode *node) {
    ((CAIObject *)this)->UpdateFromGameObject(node);
    health = *(float *)((char *)((CAIFilterObject *)((CAIObject *)this)->filter)->bsgo + 20);
}

void CAIPlayerObject::PreUpdate() {
    CAIObjectInterface *self = this;
    self->UpdateFromGameObject((IMovingSceneNode *)self->GetSceneNode());
    ((CAIObject *)this)->PreUpdate();
}

void CAIPlayerObject::Update(float dt) {
    ((CAIObject *)this)->Update(dt);
    UpdateStatus();
    if (!(g_aigAIFilterGlobalObject.time <= nextFieldOfViewCheck)) {
        for (CAIObject *other = CAIObject::GetGlobalListStart(); other; other = other->nextGlobal) {
            if (other != (CAIObject *)this && other->IsObjectInFieldOfView(*(CAIObject *)this, 5.0f, 180.0f)) {
                BSObject *targetScript = ((CAIObject *)this)->targeting.GetTargetScript();
                BSObjectTriggerEvent(other->filter->scriptObject, 218, targetScript, 0, 0);
            }
        }
        nextFieldOfViewCheck = g_aigAIFilterGlobalObject.time + 0.2f;
        if (!g_bInMultiplayerMode || g_bCoopMultiplayer)
            ((CAIObject *)this)->areaPathFinding.UpdateOffAStarPosition();
    }
    if (!g_bInMultiplayerMode || g_bCoopMultiplayer) {
        CCoverpointBehavior::UpdateCoverpointsOutOfArea(GetSceneNode());
        AIPathFinding::UpdateAStarNodesOutOfArea();
    }
}

void CAIPlayerObject::Init() {
    ((CAIObject *)this)->Init();
    SetInitialStatus();
}

void CAIPlayerObject::SetInitialStatus() {
    CAIObject *self = (CAIObject *)this;
    self->SetAIStatusVar((aistatus_var)0, (int)(15.0f * (health / maxHealth) + 0.999));
    self->SetAIStatusVar((aistatus_var)1, self->unknown599);
    self->SetAIStatusVar((aistatus_var)2, 4);
    self->SetAIStatusVar((aistatus_var)3, 1);
    self->SetAIStatusVar((aistatus_var)4, self->team);
    self->SetAIStatusVar((aistatus_var)6, 0);
}

void CAIPlayerObject::UpdateStatus() {
    CAIObject *self = (CAIObject *)this;
    self->SetAIStatusVar((aistatus_var)0, (int)(15.0f * (health / maxHealth) + 0.999));
    self->SetAIStatusVar((aistatus_var)2, 4);
    self->SetAIStatusVar((aistatus_var)3, 1);
}

bool CAIPlayerObject::IsCrouching() const {
    return g_scene.GetPlayer(0)->IsPlayerCrouching();
}

ISceneNode *CAIPlayerObject::GetSceneNode() { return sceneNode; }

void CAIPlayerObject::GetLookDirection(CVector3 &forward, CVector3 &position) {
    sceneNode->GetForward(forward);
    sceneNode->GetPosition(position);
}

void *CAIPlayerObject::AsAIPlayerObject() { return this; }

int CAIPlayerObject::IsPlayer() const { return 1; }
