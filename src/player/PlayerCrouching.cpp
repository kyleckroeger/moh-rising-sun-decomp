// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
#include "CameraShake.h"
int CPlayerObject::IsPlayerCrouching() const {
    int crouching = 0;
    if (pathBits.meleeImmune || !(field_270 >= 0.86f)) crouching = 1;
    return crouching;
}
