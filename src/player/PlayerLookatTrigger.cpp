// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
#include "CameraShake.h"
void CPlayerObject::HandleLookatTrigger(BSObject *trigger, float degrees, float value) {
    if (trigger) {
        lookatTrigger = trigger;
        lookatAngle = degrees * 0.0174532942f;
        lookatValue = value;
    } else {
        lookatTrigger = trigger;
        lookatValue = lookatAngle = 0.0f;
    }
}
