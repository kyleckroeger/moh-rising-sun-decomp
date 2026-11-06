// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original Soldier_object.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "Weapon.h"

class CWeaponFactory {
  public:
    void DestroyWeapon(CWeapon *);
};
extern CWeaponFactory g_WeaponFactory;
// Shared strings of the original pool, mapped to their addresses in the manifest.
static const char nameLeftHand[] __attribute__((section(".rodata.name_lt_hand"), aligned(4))) = { 'l', 't', '_', 'h', 'a', 'n', 'd', 0 };
static const char nameRightHand[] __attribute__((section(".rodata.name_rt_hand"), aligned(4))) = { 'r', 't', '_', 'h', 'a', 'n', 'd', 0 };
static const char nameProp[] __attribute__((section(".rodata.name_prop"), aligned(4))) = { 'J', 'O', '_', 'p', 'r', 'o', 'p', 0 };
static const char nameGun[] __attribute__((section(".rodata.name_gun"), aligned(4))) = { 'J', 'O', '_', 'g', 'u', 'n', 0 };

void CSoldierObject::RemoveWeapons() {
    CAnimObjectInterface *self = (CAnimObjectInterface *)this;
    self->DetachObject((char *)nameLeftHand);
    self->DetachObject((char *)nameRightHand);
    self->DetachObject((char *)nameProp);
    self->DetachObject((char *)nameGun);
    for (int i = 0; i < 4; i++) {
        if (weapons[i]) {
            if (g_scene.IsNodeInScene((ISceneNode *)weapons[i]))
                g_scene.Remove(*(ISceneNode *)weapons[i]);
            g_WeaponFactory.DestroyWeapon(weapons[i]);
            weapons[i] = 0;
        }
    }
    if (otherWeapon) {
        g_WeaponFactory.DestroyWeapon(otherWeapon);
        otherWeapon = 0;
    }
}
