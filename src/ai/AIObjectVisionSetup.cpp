// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Fragments of the original AIObject.cpp; member names are descriptive.
#include "AITargeting.h"
#include "AIMovement.h"
#include "BSGameObject.h"
#include "ScriptRuntime.h"
#include <stdio.h>
#include <string.h>

extern int g_iMsgVerbosityLevel;

int CAIObject::ComputeStatusLOSValue(CAITarget *target) {
    float range = ((AIScriptDataView *)scriptObject->gameObject->GetScriptData())->visionRange;
    bool crouching = false;
    CAIObject *object = target->aiObject;
    if (object) {
        if (object->inShadow)
            range *= object->shadowAmount;
        crouching = ((CAIObjectInterface *)object)->IsCrouching();
        if (crouching)
            range *= 0.75f;
    }
    if (((CAIObjectInterface *)this)->IsObjectInLineOfSight(target->GetPosition(), range, crouching, target))
        return 15;
    return 0;
}

// Descriptive helper: the teams each team treats as enemies.
static inline bool AreEnemyTeams(unsigned char team, unsigned char other) {
    switch (team) {
    case 1:
        switch (other) {
        case 0:
        case 1:
            return false;
        case 2:
            return true;
        }
        break;
    case 2:
        switch (other) {
        case 0:
        case 2:
            return false;
        case 1:
            return true;
        }
        break;
    case 0:
        switch (other) {
        case 0:
            return false;
        case 1:
        case 2:
            return true;
        }
        break;
    }
    return false;
}

int CAIObject::ComputeStatusIsEnemy(CAITarget *target) {
    CAIObject *object = target->aiObject;
    if (object) {
        if (AreEnemyTeams(team, object->team))
            return 15;
        return 0;
    }
    return 15;
}

CAIObject *CAIObject::GetNextVisionTarget() {
    CAIObject *start = GetGlobalListStart();
    CAIObject *current = visionTarget.aiObject;
    CAIObject *object = start;
    if (current)
        object = current;
    do {
        object = object->nextGlobal;
        if (!object)
            object = start;
        if (object != this && object->status.MatchStatusList(visionMatchList, visionMatchCount)) {
            visionTarget.aiObject = object;
            return object;
        }
    } while (!visionTarget.aiObject ? object != start : object != visionTarget.aiObject);
    return 0;
}

int CAIObject::IsObjectInVisionDistance(const CVector3 &point, float range) const {
    if (!(range > 0.001f)) {
        if (g_iMsgVerbosityLevel > 0)
            printf("Warning: IOIVD - Vision distance is zero, changing to default\n");
        range = 50.0f;
    }
    return position.Distance(point) <= range;
}

void CAIObject::SetupFieldOfViewModeObject(const CAITarget &target, float range, float angle) {
    fieldOfViewTarget = target;
    fieldOfViewObjectRange = range;
    fieldOfViewObjectAngle = angle * 0.0174532942f;
}

void CAIObject::SetupFieldOfViewModeList(aistatus_match *list, int count, float range, float angle) {
    memcpy(visionMatchList, list, count * sizeof(aistatus_match));
    visionMatchCount = count;
    visionTarget.Nullify();
    if (!(range > 0.001f)) {
        if (g_iMsgVerbosityLevel > 0)
            printf("Warning: SFOVML - Vision distance is zero, changing to default\n");
        range = 50.0f;
    }
    fieldOfViewListRange = range;
    if (!(angle > 0.001f))
        angle = 90.0f;
    fieldOfViewListAngle = angle * 0.0174532942f;
}

void CAIObject::SetupFieldOfViewCorpse(float range, float angle) {
    if (!(range > 0.001f)) {
        if (g_iMsgVerbosityLevel > 0)
            printf("Warning: SFOVC - Vision distance is zero, changing to default\n");
        range = 50.0f;
    }
    fieldOfViewCorpseRange = range;
    fieldOfViewCorpseAngle = angle * 0.0174532942f;
}

CAITarget *CAIObject::CheckFieldOfViewModeObject() {
    CAITarget *candidate = &fieldOfViewTarget;
    CAIObject *object = fieldOfViewTarget.aiObject;
    float angle = fieldOfViewObjectAngle;
    float range = fieldOfViewObjectRange;
    if (object->inShadow)
        range *= object->shadowAmount;
    if (((CAIObjectInterface *)object)->IsCrouching())
        range *= 0.75f;
    if (IsObjectInFieldOfView(object->position, range, angle, ((CAIObjectInterface *)object)->IsCrouching(), 0)) {
        CAITargeting *module = &targeting;
        if (candidate->Matches(module->GetTarget())) {
            CVector3 location = candidate->GetPosition();
            module->SetLastSeenTargetPosition(location);
            module->visionVisible = 1;
        }
        return candidate;
    }
    return 0;
}

int CAIObject::CheckFieldOfViewModeList(CAITarget &out) {
    const float crouchScale = 0.75f;
    CAIObject *first = visionTarget.aiObject;
    CAIObject *object;
    for (int i = 0; i <= 0 && (object = GetNextVisionTarget()); i++) {
        float range = fieldOfViewListRange;
        float angle = fieldOfViewListAngle;
        if (object->inShadow)
            range *= object->shadowAmount;
        if (((CAIObjectInterface *)object)->IsCrouching())
            range *= crouchScale;
        if (IsObjectInFieldOfView(object->position, range, angle, ((CAIObjectInterface *)object)->IsCrouching(), 0)) {
            out.SetAsAITarget(object);
            return 1;
        }
        if (object == first)
            break;
    }
    return SearchNonAITargetsInFieldOfView(out);
}
