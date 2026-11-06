// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original Soldier_object.cpp; member names are descriptive.
#include "SceneObjects.h"

void CSoldierObject::SetCurrentGag(int gag, int type) {
    currentGag = gag;
    currentGagType = type;
    if (!type)
        SaveGagState();
}
