// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
#include "profile.h"
#include <map>
#include "CameraShake.h"
#include "GameSettings.h"
#include "SceneObjects.h"
#include "ScriptRuntime.h"
typedef _STL::map<int, CPlayerWeaponObject *> PlayerWeaponMap;
// CPlayerObject's view keeps the map as raw storage so that other users need no STL headers.
#define WEAPONS ((PlayerWeaponMap *)weaponMap)
void CPlayerObject::StoreSaveData() {
    g_GameSettings.SetCurrentHealth(*(float *)script->gameObject->GetScriptData(), playerIndex);
    g_GameSettings.SetCurrentWeapon(GetCurrentWeapon(), playerIndex);
    for (PlayerWeaponMap::iterator it = WEAPONS->begin(); it != WEAPONS->end(); ++it) {
        CWeapon *weapon = (*it).second->weapon;
        g_GameSettings.SetCurrentAmmo((*it).first, weapon->loadedAmmo, weapon->reserveAmmo, playerIndex);
        g_GameSettings.SetIfCurrentWeaponSelectable((*it).first, (*it).second->IsSelectable(), playerIndex);
    }
    if (thrownWeapon) g_GameSettings.SetCurrentAmmo(thrownWeapon->crc, thrownWeapon->loadedAmmo, thrownWeapon->reserveAmmo, playerIndex);
}
void CPlayerObject::RestoreSaveData() {
    float *health = (float *)script->gameObject->GetScriptData();
    *health = g_GameSettings.GetCurrentHealth(playerIndex);
    int current;
    g_GameSettings.GetCurrentWeapon(current, playerIndex);
    for (PlayerWeaponMap::iterator it = WEAPONS->begin(); it != WEAPONS->end(); ++it) {
        unsigned int loaded, reserve;
        g_GameSettings.GetCurrentAmmo((*it).first, loaded, reserve, playerIndex);
        (*it).second->SetAmmo(loaded, reserve);
        int selectable = g_GameSettings.GetIfCurrentWeaponSelectable((*it).first, playerIndex);
        CPlayerWeaponObject *object = GetWeaponByCRC((*it).first);
        if (object) {
            object->enabled = selectable;
            if (object->IsSelectable() && (*it).first == current) {
                CycleWeapon(1, (*it).first);
                object->state = 3;
                ((ISceneNode *)this)->TriggerScriptEvent(164, (void *)(*it).first, false);
            }
        }
    }
    if (thrownWeapon) {
        unsigned int loaded, reserve;
        g_GameSettings.GetCurrentAmmo(thrownWeapon->crc, loaded, reserve, playerIndex);
        thrownWeapon->SetAmmo(loaded, reserve);
        int total = loaded + reserve;
        ((ISceneNode *)this)->TriggerScriptEvent(25, (void *)total, false);
    }
}
