// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original anim_obj.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "EAGLTransform.h"

void CAnimObject::MoveTo(const COORD3 &target, float speed) { MoveTo(*(const CVector3 *)&target, speed); }

void CAnimObject::SetAnimCallback(EAnimCallback callback) {
    pendingCallback = callback;
    if (callback == -1)
        animCallback = callback;
}

void CAnimObject::StartAnimCallback(const float &time) {
    int pending = pendingCallback;
    if (pending == -1)
        return;
    pendingCallback = -1;
    animCallback = pending;
    if (pending == 2) {
        callbackStart = time;
        callbackLast = time;
    }
}

void CAnimObject::UpdateAnimCallback(const float &time) {
    switch (animCallback) {
    case 0:
        Pathing_cb(time);
        break;
    case 1:
        PathingFacing_cb(time);
        break;
    case 2:
        Adjusting_cb(time);
        break;
    case 3:
        TrackTarget_cb(time);
        break;
    }
}
