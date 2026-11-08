// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original static_obj.cpp; member names are descriptive.
#include "SceneObjects.h"

void CStaticObject::BeginPathMotionUpdate(float) {
    pathPosition = transform.GetPos();
    pathPreviousSpeed = pathSpeed;
    flags.scriptMoving = 1;
    UpdatePathMotionDestAndSpeed();
}
