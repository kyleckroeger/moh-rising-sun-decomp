// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original hierobject.cpp; member names are descriptive.
#include "SceneObjects.h"

void CHierObject::CommitAnimMotionUpdate() {
    if (motionLoop != 0 && motionRate != 0.0f) {
        orientation = scriptTargetOrientation;
        attachOffset += attachDelta;
        animPosition = attachOffset;
        animOrientation = orientation;
    }
}
