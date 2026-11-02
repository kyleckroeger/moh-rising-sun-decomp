// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original anim_obj.cpp; member names are descriptive.
#include "SceneObjects.h"

void CAnimObject::CommitUpdate() {
    animRemaining -= animStep;
    if (!(animRemaining <= 0.0001f))
        animRate = 1.0f / animRemaining;
    DetectGround();
    if (hurtlingBlocked) {
        transform.SetPos(previousPosition);
        DetectGround();
    }
    previousPosition = transform.GetPos();
    yawCopy = yaw;
    committedOffset = pendingOffset;
    committedRotation = pendingRotation;
}
