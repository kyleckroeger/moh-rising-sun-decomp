// AI-assisted reconstruction from GR8E69; see docs/Weapons.md.
#include "CameraShake.h"
unsigned int CPlayerWeaponObject::IsSelectable() const {
    if (weapon->weaponClass == 7) return weapon->loadedAmmo + weapon->reserveAmmo != 0;
    return (flags >> 29) & 1;
}
