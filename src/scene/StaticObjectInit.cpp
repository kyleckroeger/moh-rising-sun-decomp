// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original static_obj.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "Volume.h"

void CStaticObject::Init() {
    orientation.SetFromMatrix(transform);
    flags.scriptRotating = 0;
    flags.scriptMoving = 0;
    flags.drawEnabled = 1;
    flags.scriptScaling = 0;
    scriptScaleBase = 1.0f;
    velocity.Zero();
    pathState = 0;
    pathTurnSlowdown = 0.5f;
    flags.allowCollisionDamage = 0;
    flags.pathFlag1 = 0;
    maxLightPriority = -1;
}

