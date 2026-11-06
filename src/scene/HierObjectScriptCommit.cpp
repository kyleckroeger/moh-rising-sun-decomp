// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original hierobject.cpp; member names are descriptive.
#include "SceneObjects.h"

void CHierObject::CommitScriptMotionUpdate() {
    if (flags.scriptMoving) {
        float time = scriptMoveTime -= scriptMoveStep;
        if (!(time >= 0.0f))
            time = 0.0f;
        scriptMoveTime = time;
        attachOffset += attachDelta;
    }
    if (flags.scriptRotating) {
        orientation = scriptTargetOrientation;
        float time = scriptRotateTime -= scriptRotateStep;
        if (!(time >= 0.0f))
            time = 0.0f;
        scriptRotateTime = time;
    }
}
