// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original static_obj.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "ScriptRuntime.h"
#include "TriggerObject.h"

static inline CVector3 KeyPosition(const MotionFrame *frames, int key) { return frames[key].position; }
static inline CQuaternion KeyRotation(const MotionFrame *frames, int key) { return frames[key].rotation; }

void CStaticObject::StartMotionPlayback(int start, int end, EBSEventEnum event) {
    if (motionFrames) {
        if (start < 0)
            start = 0;
        if (start >= motionLoop)
            start = motionLoop - 1;
        if (end < 0)
            end = 0;
        if (end >= motionLoop)
            end = motionLoop;
        motionPlayStart = start;
        motionTimer = start;
        motionKey = end;
        motionPlayEvent = event;
        if (script) {
            int relative = 0;
            TriggerObject *trigger = script->nativeObject;
            if (!flags.hasMotion && trigger->GetBool("MotionIsPlayerRelative"))
                relative = 1;
            flags.unknown_20 = relative;
        } else {
            flags.unknown_20 = 0;
        }
        motionStartOrientation.SetFromMatrix(transform);
        motionStartPosition = transform.GetPos();
        if (!flags.hasMotion) {
            {
                CQuaternion rotation = KeyRotation(motionFrames, motionTimer);
                animOrientation = rotation;
            }
            animPosition = KeyPosition(motionFrames, motionTimer);
            animOrientation.GetMatrix(transform);
            transform.SetPos(animPosition);
        }
        flags.motionPlaying = 1;
    }
}
