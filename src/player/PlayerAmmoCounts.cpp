// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
#include "CameraShake.h"
#include "Weapon.h"
int CPlayerObject::GetCurrentWeaponClipAmoCount() const {
    if (pathBits.meleeImmune) return mountedWeapon->loadedAmmo;
    else if (currentWeapon) return currentWeapon->weapon->loadedAmmo;
    else return 0;
}
int CPlayerObject::GetCurrentWeaponReserveAmoCount() const {
    if (pathBits.meleeImmune) return mountedWeapon->reserveAmmo;
    else if (currentWeapon) return currentWeapon->weapon->reserveAmmo;
    else return 0;
}
