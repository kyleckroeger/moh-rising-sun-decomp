// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
#include "CameraShake.h"
#include "ScriptRuntime.h"
#include "BSGameObject.h"
extern int g_bInMultiplayerMode;
extern int g_bCoopMultiplayer;
// The player's script data block; only what CycleWeapon writes.
struct PlayerScriptData {
    unsigned char unknown_0[4];
    int weaponCrc;
    CPlayerWeaponObject *weapon;
};
int CPlayerObject::CycleWeapon(int direction, int crc) {
    CPlayerWeaponObject *weapon;
    if (!currentWeapon) {
        weapon = weaponList;
        do {
            if (weapon->IsSelectable() && (!crc || crc == weapon->weapon->crc)) break;
            weapon = weapon->next;
        } while (weapon != weaponList);
    } else {
        weapon = direction > 0 ? currentWeapon->next : currentWeapon->previous;
        while ((weapon != currentWeapon && !weapon->IsSelectable()) ||
               (crc && crc != weapon->weapon->crc)) {
            weapon = direction > 0 ? weapon->next : weapon->previous;
        }
    }
    if (currentWeapon) {
        if (!currentWeapon->IsSelectable() || weapon != currentWeapon) {
            currentWeapon->Shoot((EWeaponShootType)1, 0);
            CPlayerWeaponObject *held = currentWeapon;
            held->visible = 0;
            held->SetDrawDelay(1);
            currentWeapon->weapon->reloading = 0;
        }
    }
    if (weapon != currentWeapon) {
        currentWeapon = weapon;
        weapon->visible = 1;
        weapon->SetDrawDelay(1);
        PlayerScriptData *data = (PlayerScriptData *)script->gameObject->GetScriptData();
        data->weapon = currentWeapon;
        CWeapon *held = currentWeapon->weapon;
        data->weaponCrc = held->crc;
        if (g_bInMultiplayerMode || g_bCoopMultiplayer) {
            int slot = ((CAnimatedPlayer *)body)->GetWeaponSlot(held);
            ((CSoldierObject *)body)->SetCurrentWeapon(slot, 0, true);
        }
        return 1;
    }
    currentWeapon = 0;
    return 0;
}
