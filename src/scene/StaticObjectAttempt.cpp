// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original static_obj.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "Volume.h"

void CStaticObject::AttemptUpdate(float dt) {
    if (pathState > 1) {
        ((CStaticObjectInterface *)this)->AttemptPathMotionUpdate(dt);
    } else if (ScriptMoving() || ScriptRotating() || ScriptScaling()) {
        if (!flags.attached)
            ((CStaticObjectInterface *)this)->AttemptScriptMotionUpdate(dt);
    } else if (flags.motionPlaying) {
        ((CStaticObjectInterface *)this)->AttemptAnimMotionUpdate(dt);
    }
}
void CStaticObject::CommitScriptMotionUpdate() {
    pathPosition = transform.GetPos();
    if (flags.scriptMoving) {
        float time = scriptMoveTime -= scriptMoveStep;
        if (!(time >= 0.0f))
            time = 0.0f;
        scriptMoveTime = time;
    }
    if (flags.scriptRotating) {
        orientation = scriptTargetOrientation;
        float time = scriptRotateTime -= scriptRotateStep;
        if (!(time >= 0.0f))
            time = 0.0f;
        scriptRotateTime = time;
    }
    if (flags.scriptScaling) {
        scriptScaleBase += scriptScaleStep * scriptScaleTarget;
        float time = scriptScaleRate -= scriptScaleStep;
        if (!(time >= 0.0f))
            time = 0.0f;
        scriptScaleRate = time;
    }
}
