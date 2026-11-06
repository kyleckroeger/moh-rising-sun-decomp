// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
#include "CameraShake.h"
#include "MPWeaponSet.h"
extern int g_bInMultiplayerMode;
float MathFunRandomReal(float, float);
int CPlayerObject::CanPlayerSwitchWeapon() {
    CPlayerWeaponObject *weapon = weaponList;
    do {
        if (weapon->IsSelectable() && weapon != currentWeapon) return 1;
        weapon = weapon->next;
    } while (weapon != weaponList);
    return 0;
}
