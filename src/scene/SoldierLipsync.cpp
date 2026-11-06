// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original Soldier_object.cpp; member names are descriptive.
#include "SceneObjects.h"

extern "C" void MUTEX_lock(void *);
extern "C" void MUTEX_unlock(void *);

void CSoldierObject::SpecifyTurnAngle(float angle) {
    turnAnglePending = 1;
    turnAngle = angle;
}

float CSoldierObject::GetLipsynchBlendWeight() {
    void *mutex = lipsyncMutex;
    MUTEX_lock(mutex);
    float weight = lipsyncBlendWeight;
    MUTEX_unlock(mutex);
    return weight;
}
