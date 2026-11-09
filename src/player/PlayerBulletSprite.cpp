// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
#include "CameraShake.h"
void *CPlayerObject::GetCurrentBulletSprite() const {
    if (pathBits.meleeImmune) return mountedWeapon->GetBulletSprite();
    if (currentWeapon) return currentWeapon->weapon->GetBulletSprite();
    return 0;
}
