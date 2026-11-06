// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original static_obj.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "Volume.h"

void CStaticObject::ScaleFromScript(float scale, float time, EBSEventEnum event) {
    scriptScaleEvent = event;
    scriptScaleTarget = scale;
    flags.scriptScaling = 1;
    scriptScaleRate = time / scale;
}

