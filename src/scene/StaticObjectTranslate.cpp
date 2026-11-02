// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original static_obj.cpp; member names are descriptive.
#include "SceneObjects.h"

void CStaticObject::TranslateFromScript(const CVector3 &rate, const CVector3 &distance, EBSEventEnum event) {
    scriptMoveEvent = event;
    flags.scriptMoving = 1;
    if (distance.x == 0.0f && distance.y == 0.0f && distance.z == 0.0f)
        scriptMoveTime = 3.4028235e38f;
    else
        scriptMoveTime = CalculateMovementTime(rate, distance);
    velocity = rate;
    pathPosition = transform.GetPos();
}
