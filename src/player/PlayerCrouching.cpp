// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
#include "CameraShake.h"
bool CPlayerObject::IsPlayerCrouching() const {
    bool crouching = false;
    if (pathBits.meleeImmune || !(field_270 >= 0.86f)) crouching = 1;
    return crouching;
}
