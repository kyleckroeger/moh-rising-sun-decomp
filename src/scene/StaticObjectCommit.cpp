// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original static_obj.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "Volume.h"

void CStaticObject::CommitAnimMotionUpdate() {
    if (motionLoop != 0 && motionRate != 0.0f) {
        motionRemaining -= motionStep;
        if (!(motionRemaining <= 0.0001f))
            motionSampleRate = 1.0f / motionRemaining;
        animPosition = transform.GetPos();
        animOrientation.SetFromMatrix(transform);
    }
}


void CStaticObject::CommitPathMotionUpdate() {
    pathPosition = transform.GetPos();
    pathPreviousSpeed = pathSpeed;
    UpdatePathMotionDestAndSpeed();
}
