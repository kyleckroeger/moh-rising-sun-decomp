// AI-assisted reconstruction from GR8E69; see docs/CameraShake.md.
#include "CameraShake.h"
void CPlayerObject::CCameraShake::Update(float dt) {
    if (rate != 0.0f) {
        current += rate * dt;
        if ((!(rate >= 0.0f) && current < target) || (!(rate <= 0.0f) && !(current <= target))) {
            current = target;
            rate = 0.0f;
        }
    } else if (duration != 100000.0f && !(elapsed <= duration)) {
        SetShake(0.0f, fadeTime);
        return;
    }
    elapsed += dt;
}
