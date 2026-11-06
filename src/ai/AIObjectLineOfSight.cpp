// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Fragments of the original AIObject.cpp; member names are descriptive.
#include "AITargeting.h"
#include "AIFilterGlobal.h"
#include "SceneNode.h"
#include "ScriptRuntime.h"
#include "BSGameObject.h"
#include <stdio.h>
#include <string.h>

extern "C" float sqrtf(float);
extern "C" float cosf(float);
extern int g_iMsgVerbosityLevel;

// Descriptive helper: scale to unit length, leaving a zero vector alone.
static inline void NormalizeVector(CVector3 &v) {
    float length = sqrtf(v.LengthSquared());
    if (length != 0.0f)
        v *= 1.0f / length;
}

int CAIObject::CheckFieldOfViewCorpse(CAITarget &out) {
    const float crouchScale = 0.75f;
    for (CAIObject *object = GetGlobalListStart(); object; object = object->nextGlobal) {
        if (object->status.GetVarValue((aistatus_var)11)) {
            float range = fieldOfViewCorpseRange;
            float angle = fieldOfViewCorpseAngle;
            if (object->inShadow)
                range *= object->shadowAmount;
            if (((CAIObjectInterface *)object)->IsCrouching())
                range *= crouchScale;
            if (IsObjectInFieldOfView(object->position, range, angle, ((CAIObjectInterface *)object)->IsCrouching(), 0)) {
                out.SetAsAITarget(object);
                return 1;
            }
        }
    }
    return 0;
}

int CAIObject::IsObjectInFieldOfView(const CVector3 &point, float range, float angle, bool crouching, ISceneNode *ignore) {
    CVector3 direction, eye;
    if (statusFlags & 8)
        return 0;
    ((CAIObjectInterface *)this)->GetLookDirection(direction, eye);
    NormalizeVector(direction);
    return IsObjectInFieldOfView(eye, direction, point, range, angle, crouching, ignore);
}

int CAIObject::IsObjectInFieldOfView(const CVector3 &eye, const CVector3 &direction, const CVector3 &point, float range, float angle, bool crouching, ISceneNode *ignore) {
    const float one = 1.0f;
    const float x = point.x, y = point.y;
    float z = point.z + 0.8f;
    CVector3 raised(x, y, z);
    CVector3 toPoint = raised;
    toPoint -= eye;
    float length = sqrtf(toPoint.LengthSquared());
    if (length != 0.0f)
        toPoint *= one / length;
    float dot = toPoint.x * direction.x + toPoint.y * direction.y;
    dot += toPoint.z * direction.z;
    float limit = cosf(angle);
    if (IsObjectInVisionDistance(point, range))
        return dot < limit ? 0 : IsObjectOccluded(eye, raised, crouching, ignore);
    return 0;
}

int CAIObject::IsObjectOccluded(const CVector3 &eye, const CVector3 &point, bool crouching, ISceneNode *ignore) {
    float standingHeights[2] = {0.8f, 1.2f};
    float crouchingHeights[2] = {0.8f, 0.1f};
    float *heights = crouching ? crouchingHeights : standingHeights;
    int count = 2;
    for (int i = 0; i < count; i++) {
        CVector3 raised(point.x, point.y, point.z);
        CVector3 &r = raised;
        r.z = point.z + heights[i];
        if (!CAIFilterGlobal::CheckVisionLineToEnvironment(eye, r, sceneNode, ignore))
            return 1;
    }
    return 0;
}

void CAIObject::SetupLineOfSightModeObject(const CAITarget &target, float range) {
    fieldOfViewTarget = target;
    fieldOfViewObjectRange = range;
}

void CAIObject::SetupLineOfSightModeList(aistatus_match *list, int count, float range) {
    memcpy(visionMatchList, list, count * sizeof(aistatus_match));
    visionMatchCount = count;
    visionTarget.Nullify();
    if (!(range > 0.001f)) {
        if (g_iMsgVerbosityLevel > 0)
            printf("Warning: SLOSML - Vision distance is zero, changing to default\n");
        range = 50.0f;
    }
    fieldOfViewListRange = range;
}

// Also emitted out of line in the original (see AIObjectLineOfSightOutOfLine); here it is
// inlined into its two callers.
inline int CAIObject::IsObjectInLineOfSight(const CAITarget &target, float range) {
    bool crouching = false;
    CAIObject *object = target.aiObject;
    if (object) {
        if (object->inShadow)
            range *= object->shadowAmount;
        crouching = ((CAIObjectInterface *)object)->IsCrouching();
        if (crouching)
            range *= 0.75f;
    }
    return ((CAIObjectInterface *)this)->IsObjectInLineOfSight(target.GetPosition(), range, crouching, &target);
}

CAITarget *CAIObject::CheckLineOfSightModeObject() {
    if (!fieldOfViewTarget.HasTarget())
        return 0;
    CAITargeting *module = &targeting;
    int visible;
    if (fieldOfViewTarget.Matches(module->GetTarget())) {
        visible = IsTargetInLineOfSight(fieldOfViewObjectRange);
        if (visible) {
            CVector3 location = fieldOfViewTarget.GetPosition();
            module->SetLastSeenTargetPosition(location);
            module->visionVisible = 1;
        } else
            module->visionVisible = visible;
    } else
        visible = IsObjectInLineOfSight(fieldOfViewTarget, fieldOfViewObjectRange);
    return visible ? &fieldOfViewTarget : 0;
}

int CAIObject::CheckLineOfSightModeList(CAITarget &out) {
    CAIObject *previous = visionTarget.aiObject;
    for (int i = 0; i <= 0; i++) {
        CAIObject *object = GetNextVisionTarget();
        if (!object)
            return 0;
        CAITarget candidate(object);
        if (IsObjectInLineOfSight(candidate, fieldOfViewListRange)) {
            out.SetAsAITarget(object);
            return 1;
        }
        if (object == previous)
            break;
        previous = object;
    }
    return 0;
}

int CAIObject::IsTargetInLineOfSight(float range) {
    CAITarget current = targeting.GetTarget();
    return ((CAIObjectInterface *)this)->IsObjectInLineOfSight(current.GetPosition(), range, false, &current);
}

