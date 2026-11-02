// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
#include "CameraShake.h"
int CAnimatedPlayer::GetWeaponSlot(CWeapon *weapon) {
    CWeapon **slots = weapons;
    int crc = weapon->crc;
    for (int i = 0; i < 4; i++)
        if ((*slots++)->crc == crc) return i;
    return -1;
}
