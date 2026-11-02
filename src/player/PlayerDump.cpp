// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
#include "CameraShake.h"
#include "MPWeaponSet.h"
extern int g_bInMultiplayerMode;
float MathFunRandomReal(float, float);
void CPlayerObject::DropMPDumpObject() {
    if (currentWeapon) {
        CWeapon *weapon = currentWeapon->weapon;
        int reserve = weapon->reserveAmmo;
        int loaded = weapon->loadedAmmo;
        CreateDumpObject(localToWorld, weapon->properties->crc, false, 0, reserve + loaded, (EPowerupType)3);
    }
}
