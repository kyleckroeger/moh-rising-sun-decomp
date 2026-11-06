// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Fragments of the original AIObject.cpp; member names are descriptive.
#include "AITargeting.h"
#include "AIFilterGlobal.h"
#include "SceneNode.h"
#include "ScriptTimers.h"

extern int g_iAIObjectCount;
extern CAIObject *g_paioAIObjectListStart;
extern int Gameloop_QueryState();

void CAIObject::RemovePointersTo(CAIObject *object) {
    CAITargeting *targetingModule = &targeting;
    CAITarget current = targetingModule->GetTarget();
    if (current.aiObject == object) {
        if (Gameloop_QueryState() != 5)
            BSObjectTriggerEvent(filter->scriptObject, 65, 0, 0, true);
        targetingModule->SetTarget(0);
    }
    areaPathFinding.RemovePointersTo(object);
    CAITarget *vision = &visionTarget;
    if (visionTarget.aiObject == object)
        vision->Nullify();
    CAITarget *fieldOfViewObject = &fieldOfViewTarget;
    if (fieldOfViewTarget.aiObject == object) {
        if (Gameloop_QueryState() != 5)
            BSObjectTriggerEvent(filter->scriptObject, 65, 0, 0, true);
        fieldOfViewObject->Nullify();
    }
}

CAIObject *CAIObject::GetGlobalListStart() { return g_paioAIObjectListStart; }

void CAIObject::ResetGlobalList() {
    g_paioAIObjectListStart = 0;
    g_iAIObjectCount = 0;
}

void CAIObject::PreUpdate() {}

void CAIObject::Update(float dt) {
    lastUpdateTime = g_aigAIFilterGlobalObject.time;
    UpdateHeuristics((int)dt, false);
    areaPathFinding.Update();
}

void CAIObject::UpdateFromGameObject(IMovingSceneNode *node) {
    sceneNode = node;
    previousStatusFlags2 = statusFlags2;
    statusFlags2 &= 1;
    if (node) {
        CVector3 right, forward, up, position;
        node->GetPosition(position);
        node->GetRightward(right);
        node->GetForward(forward);
        node->GetUpward(up);
        Physics()->UpdateOrientationFrom(right, forward, up);
        Physics()->UpdatePositionFrom(position);
    }
}

void CAIObject::Init() { AddToGlobalList(); }

void CAIObject::UpdateHeuristics(int dt, bool force) {
    int count;
    CBehavior **list;
    int changed = 0;
    switch (highLevelState) {
    case 1: count = behaviorLists[0].count; list = behaviorLists[0].behaviors; break;
    case 2: count = behaviorLists[1].count; list = behaviorLists[1].behaviors; break;
    case 3: count = behaviorLists[2].count; list = behaviorLists[2].behaviors; break;
    case 4: count = behaviorLists[3].count; list = behaviorLists[3].behaviors; break;
    default: return;
    }
    for (int i = 0; i < count; i++) {
        CBehavior *behavior = list[i];
        if (behavior->enabled)
            changed = changed || behavior->Update(dt, force);
    }
    if (changed)
        BSObjectTriggerEvent(filter->scriptObject, 268, 0, 0, true);
}

void SendHighLevelStateTransitionEvents(BSObject *object, EAIHighLevelState from, EAIHighLevelState to) {
    int send = 0;
    int event = 0;
    if (from == 1) {
        if (to == 2) { send = 1; event = 424; }
        else if (to == 3) { send = 1; event = 425; }
        else if (to == 4) { send = 1; event = 426; }
    } else if (from == 2) {
        if (to == 1) { send = 1; event = 427; }
        else if (to == 3) { send = 1; event = 428; }
        else if (to == 4) { send = 1; event = 429; }
    } else if (from == 3) {
        if (to == 2) { send = 1; event = 431; }
        else if (to == 4) { send = 1; event = 430; }
    } else if (from == 4) {
        if (to == 3) { send = 1; event = 432; }
    }
    if (send)
        BSObjectTriggerEvent(object, event, 0, 0, true);
}

void CAIObject::SetHighLevelState(EAIHighLevelState state) {
    if (highLevelState != state) {
        SendHighLevelStateTransitionEvents(scriptObject, highLevelState, state);
        EAIHighLevelState previous = highLevelState;
        highLevelState = state;
        ((CAIObjectInterface *)this)->OnStateChange(previous, state);
        UpdateHeuristics(0, true);
    }
}

int CAIObject::ChooseTarget(float) { return 0; }
