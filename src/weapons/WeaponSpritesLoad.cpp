// AI-assisted reconstruction from GR8E69; see docs/Weapons.md.
#include "profile.h"
#include <map>
#include "Weapon.h"
#include "CameraShake.h"
extern "C" int strcmp(const char *, const char *);
extern "C" char *strcpy(char *, const char *);
struct PickupInfo;
class CStaticObjectFactory {
  public:
    static CStaticObject *AllocateStaticObject(int, int, PickupInfo *);
    static void DeallocateStaticObject(CStaticObject *);
};
class CGameObjectFactory {
  public:
    CGameObjectFactory();
    void Init(int, int, float);
    void Reset();
    void Shutdown();
    unsigned char unknown_0[0x20];
};
extern CStaticObject *g_pStaticObjectMuzzleFlashNPC;
extern CStaticObject *g_pStaticObjectMuzzleFlashPlayer;
// A 28-byte element of the array at +0x10; only its table at +24 is used here.
class WeaponFactoryEntry {
  public:
    unsigned char unknown_0[24];
    virtual ~WeaponFactoryEntry();
};
class CWeaponFactory {
  public:
    _STL::map<int, WeaponProperties_struct *> properties; // +0x00
    WeaponFactoryEntry *entries;                          // +0x10
    unsigned char unknown_14[0x20 - 0x14];
    CGameObjectFactory objects;                           // +0x20
    int initialized;                                      // +0x40
    CWeaponFactory();
    void Init();
    void Shutdown();
    WeaponProperties_struct *GetWeaponPropertiesByCRC(int, int *);
    void LoadWeaponParameters();
    void LoadWeaponGUISprites();
    void LoadWeaponObjects();
    void DeleteWeaponGUISprites();
    void DeleteWeaponParameters();
};
void CWeaponFactory::LoadWeaponGUISprites() {
    char path[64];
    typedef _STL::map<int, WeaponProperties_struct *>::iterator Iterator;
    for (Iterator it = properties.begin(); it != properties.end(); ++it) {
        WeaponProperties_struct *record = (*it).second;
        const char *name = record->iconName;
        record->icon = 0;
        if (name[0]) {
            for (Iterator other = properties.begin(); other != it; ++other) {
                if (!strcmp(name, (*other).second->iconName)) {
                    record->icon = (*other).second->icon;
                    break;
                }
            }
            if (!record->icon) {
                strcpy(path, "data\\");
                strcpy(path + 5, name);
                record->icon = new ("source/weapons/weaponfactory.cpp:400") CSprite(name, CSprite::SpriteLoadUnknown1);
            }
        }
    }
}
