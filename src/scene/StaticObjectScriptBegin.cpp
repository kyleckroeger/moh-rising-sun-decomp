// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original static_obj.cpp; member names are descriptive.
#include "SceneObjects.h"

void CStaticObject::BeginScriptMotionUpdate(float) {
    pathPosition = transform.GetPos();
    if (flags.scriptMoving && !(scriptMoveTime > 0.0f)) {
        TriggerScriptEvent((unsigned short)scriptMoveEvent, 0, 0);
        scriptMoveTime = 0.0f;
        flags.scriptMoving = 0;
        velocity.Zero();
    }
    if (flags.scriptRotating) {
        orientation.Normalize();
        if (!(scriptRotateTime > 0.0f)) {
            TriggerScriptEvent((unsigned short)scriptRotateEvent, 0, 0);
            scriptRotateTime = 0.0f;
            flags.scriptRotating = 0;
            scriptRotationDelta.Set(0.0f, 0.0f, 0.0f, 1.0f);
        }
    }
    if (flags.scriptScaling && !(scriptScaleRate > 0.0f)) {
        TriggerScriptEvent((unsigned short)scriptScaleEvent, 0, 0);
        scriptScaleRate = 0.0f;
        scriptScaleTarget = 0.0f;
        flags.scriptScaling = 0;
    }
}
