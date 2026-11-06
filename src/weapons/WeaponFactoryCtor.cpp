// AI-assisted reconstruction from GR8E69; see docs/Weapons.md.
#include "profile.h"
#include <map>
#include "Weapon.h"
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
CWeaponFactory::CWeaponFactory() { initialized = 0; }
