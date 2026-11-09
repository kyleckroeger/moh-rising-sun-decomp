// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
#include "profile.h"
#include <map>
#include <string.h>
#include "CameraShake.h"
#include "SceneObjects.h"
#include "SceneNode.h"
#include "Weapon.h"
#include "GameSettings.h"
#include "Toilet.h"
#include "AITargeting.h"
#include "BSGameObject.h"
typedef _STL::map<int, CPlayerWeaponObject *> PlayerWeaponMap;
#define WEAPONS ((PlayerWeaponMap *)weaponMap)
enum EObjectClass { ObjectClassUnknown = 0 };
class CAnimObjectFactory {
  public:
    CAnimObject *AllocateObject(unsigned int, EObjectClass);
};
class CWeaponFactory {
  public:
    CWeapon *CreateWeapon(int, ISceneNode *, int);
};
extern CAnimObjectFactory g_AnimObjFactory;
extern CWeaponFactory g_WeaponFactory;
extern int g_levelWeaponSetID;
void DebugMsg(const char *, ...);
struct BSObjectOwnerView { unsigned char unknown_0[32]; BSGO_Basic *object; };
// A counted table in a level resource. Field names are descriptive.
struct LevelIntTable {
    int unknown_0;
    int count;
    int values[1]; // count entries.
};
struct LevelShortTable {
    unsigned char unknown_0[10];
    short value;
};
struct LevelWeaponSet {
    unsigned char unknown_0[4];
    LevelIntTable *grenade;     // +0x04; values[0] is the grenade CRC.
    LevelIntTable *weapons;     // +0x08
    LevelIntTable *selectable;  // +0x0c
    LevelIntTable *grenadeAmmo; // +0x10
    LevelIntTable *ammo;        // +0x14
};
struct LevelBodyTable {
    LevelIntTable *bodies; // +0x00
};
#define WEAPON_SET(resource) (*(LevelWeaponSet **)((char *)(resource) + 8))
void CPlayerObject::InitMultiplayer(SMPPlayerData *data, bool respawn) {
    char team = data->restartIndex;
    aiObject->team = team;
    if (respawn) {
        LevelFileContentsStruct_ *resource = TLT_FindNextResourceByTypeAndVal((TLTResourceID)13, 0, g_levelWeaponSetID, 0);
        int count = WEAPON_SET(resource)->weapons->count;
        if (thrownWeapon) {
            thrownWeapon->Empty();
            thrownWeapon->AddAmo(((LevelShortTable *)WEAPON_SET(resource)->grenadeAmmo)->value);
            thrownWeapon->DoneReloading();
        }
        for (int i = 0; i < count; i++) {
            int crc = WEAPON_SET(resource)->weapons->values[i];
            CPlayerWeaponObject *object = (*WEAPONS->find(crc)).second;
            object->weapon->Empty();
            object->AddAmo(WEAPON_SET(resource)->ammo->values[i]);
            object->DoneReloading();
            int selectable = WEAPON_SET(resource)->selectable->values[i];
            if (selectable == 1) object->enabled = selectable;
            else {
                int disabled = 0;
                object->enabled = disabled;
            }
        }
        int slot = 0;
        for (PlayerWeaponMap::iterator it = WEAPONS->begin(); it != WEAPONS->end(); ++it) {
            ((CSoldierObject *)body)->AddWeapon(slot, (*it).second->weapon->crc, 999);
        slot++;
        }
        return;
    }
    char *playerName = name;
    strncpy(playerName, data->name, 10);
    char *end = playerName;
    int last = 10;
    int nul = 0;
    do {
        name[last] = nul;
        last--;
    } while (last >= 0 && (end[last] == ' ' || end[last] == 0));
    teamWord = (char)data->restartIndex;
    if (!body) {
        LevelFileContentsStruct_ *bodies = TLT_FindNextResourceByType((TLTResourceID)12, 0);
        CAnimatedPlayer *animated = (CAnimatedPlayer *)g_AnimObjFactory.AllocateObject(((LevelBodyTable *)WEAPON_SET(bodies))->bodies->values[(int)data->bodyIndex], (EObjectClass)3);
        body = (int)animated;
        ((CAnimObjectInterface *)animated)->SetScript(script);
        ((CAnimatedPlayer *)body)->SetPlayer(this);
        g_scene.Add(*(ISceneNode *)body, 0);
        *(int *)((char *)((BSObjectOwnerView *)script)->object->GetScriptData() + 28) = body;
        ((CAnimatedPlayer *)body)->team = teamWord;
    }
    LevelFileContentsStruct_ *resource = TLT_FindNextResourceByTypeAndVal((TLTResourceID)13, 0, g_levelWeaponSetID, 0);
    int count = WEAPON_SET(resource)->weapons->count;
    for (int i = 0; i < count; i++) {
        CPlayerWeaponObject *object = AddPlayerWeapon(WEAPON_SET(resource)->weapons->values[i]);
        if (object) ((CAnimObjectInterface *)object)->SetScript(script);
    }
    if (!thrownWeapon) {
        int crc = WEAPON_SET(resource)->grenade->values[0];
        if (crc) {
            int ammo = WEAPON_SET(resource)->grenadeAmmo->values[0];
            thrownWeapon = g_WeaponFactory.CreateWeapon(crc, (ISceneNode *)this, ammo);
            DebugMsg("Initializing player grenade CRC 0x%08x, start ammo %d \n", crc, ammo);
        } else {
            DebugMsg("Not initializing player grenade. \n");
        }
    }
    int slot = 0;
    for (PlayerWeaponMap::iterator it = WEAPONS->begin(); it != WEAPONS->end(); ++it) {
        ((CSoldierObject *)body)->AddWeapon(slot, (*it).second->weapon->crc, 999);
        slot++;
    }
    for (int j = 0; j < count; j++) {
        int enable = 1;
        if (WEAPON_SET(resource)->selectable->values[j] == 1) {
            int crc = WEAPON_SET(resource)->weapons->values[j];
            CPlayerWeaponObject *object = (*WEAPONS->find(crc)).second;
            if (respawn) object->weapon->Empty();
            object->AddAmo(WEAPON_SET(resource)->ammo->values[j]);
            object->enabled = enable;
            object->DoneReloading();
        }
    }
    if (count) {
        ((ISceneNode *)this)->TriggerScriptEvent(164, 0, false);
        CycleWeapon(1, 0);
    }
}
