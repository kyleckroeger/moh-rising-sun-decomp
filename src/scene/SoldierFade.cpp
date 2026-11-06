// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original Soldier_object.cpp; member names are descriptive.
#include "SceneObjects.h"

void CSoldierObject::FadeOut(float duration) {
    ((CObjectFadeManager *)fadeStorage)->StartFadeOut(duration, 0.5f);
    ((CAnimObject *)this)->velocity.Zero();
}

float CSoldierObject::FadeValue() const { return ((CObjectFadeManager *)fadeStorage)->FadeValue(); }
