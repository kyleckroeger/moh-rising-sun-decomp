// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original anim_obj.cpp; member names are descriptive.
#include "SceneObjects.h"

void CAnimObject::StartAnimAdjust(const CVector3 &target, float value, bool blend) {
    adjustTarget = target;
    adjustValue = value;
    adjustBlend = blend;
    ((CAnimObjectInterface *)this)->SetAnimCallback((EAnimCallback)2);
}
