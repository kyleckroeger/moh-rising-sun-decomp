// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original Soldier_object.cpp; member names are descriptive.
#include "SceneObjects.h"

int CSoldierObject::SetFaceAnimationState(unsigned short state, int) {
    faceAnimationState = state;
    faceAnimationPending = 1;
    return 0;
}
