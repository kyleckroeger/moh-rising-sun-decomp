// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original hierobject.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "ObserverTypes.h"
#include "GameSettings.h"

void CHierObject::StartMotionPlayback(int start, int end, EBSEventEnum event) {
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
        motionStartOrientation = orientation;
        motionStartPosition = attachOffset;
        flags.motionPlaying = 1;
    }
}

