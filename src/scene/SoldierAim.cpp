// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original Soldier_object.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "CameraShake.h"

void CSoldierObject::EnableAimModifier(bool enabled) {
    ((EALA::Character::AimModifier *)aimModifier)->SetEnabled(enabled);
    if (enabled)
        aiObject->aimDirty = 1;
    else
        aiObject->aimDirty = enabled;
}

void CSoldierObject::EnablePropAimModifier(bool enabled) {
    ((EALA::Character::AimModifier *)propAimModifier)->SetEnabled(enabled);
    if (enabled)
        aiObject->aimDirty = 1;
    else
        aiObject->aimDirty = enabled;
}

void CSoldierObject::ResumeAimModifier() {
    aimSuspended = 0;
    aiObject->aimDirty = 1;
}

void CSoldierObject::ResumePropAimModifier() {
    propAimSuspended = 0;
    aiObject->aimDirty = 1;
}

void CSoldierObject::SetHeadTrackPlayer(bool enabled) {
    ((EALA::Character::AimModifier *)headTrackModifier)->SetEnabled(enabled);
    headTrackPlayer = enabled;
}

void CSoldierObject::SetHeadTrans(bool enabled) {
    if (headTransState <= 1)
        headTransState = enabled;
    else if (headTransState == 2 && !enabled)
        headTransState = 3;
}
