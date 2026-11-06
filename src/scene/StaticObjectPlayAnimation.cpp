// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original static_obj.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "Volume.h"

void CStaticObject::PlayAnimation(int speed, int start, int end, EBSEventEnum event) {
    if (speed < 0) {
        if (start > end) {
            speed = -speed;
            flags.playReverse = 1;
        } else {
            speed = 0;
            flags.playReverse = 0;
        }
    } else {
        flags.playReverse = 0;
    }
    if (start < 0)
        start = 0;
    if (start >= motionFrameCount)
        start = motionFrameCount - 1;
    if (end < 0)
        end = 0;
    if (end >= motionFrameCount)
        end = motionFrameCount;
    motionSpeed = speed;
    motionFrame = start;
    motionStart = start;
    motionEnd = end;
    motionEvent = event;
}

