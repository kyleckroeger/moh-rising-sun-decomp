// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
#include "CameraShake.h"
void CPlayerObject::TransitionFOV(float target, float rate) {
    if (fov != target) {
        fovTarget = target;
        if (!(target >= fov)) fovRate = -rate;
        else fovRate = rate;
    }
}
