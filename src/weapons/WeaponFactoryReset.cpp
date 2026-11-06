// AI-assisted reconstruction from GR8E69; see docs/Weapons.md.
#include "Weapon.h"
struct PickupInfo;
class CStaticObjectFactory {
  public:
    static CStaticObject *AllocateStaticObject(int, int, PickupInfo *);
};
class CGameObjectFactory {
  public:
    void Reset();
};
extern CStaticObject *g_pStaticObjectMuzzleFlashNPC;
extern CStaticObject *g_pStaticObjectMuzzleFlashPlayer;
// Scoped view; the weapon map at +0x00 and the property storage are not declared.
class CWeaponFactory {
  public:
    unsigned char unknown_0[0x20];
    CGameObjectFactory objects; // +0x20
    unsigned char unknown_21[0x40 - 0x21];
    int initialized; // +0x40
    void Reset();
};
void CWeaponFactory::Reset() {
    objects.Reset();
    g_pStaticObjectMuzzleFlashNPC = CStaticObjectFactory::AllocateStaticObject(0xb095be01, -1, 0);
    g_pStaticObjectMuzzleFlashPlayer = CStaticObjectFactory::AllocateStaticObject(0xae37da9c, -1, 0);
}
