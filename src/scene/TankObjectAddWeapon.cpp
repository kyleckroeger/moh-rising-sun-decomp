// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original Tank_object.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "Weapon.h"

class CWeaponFactory {
  public:
    CWeapon *CreateWeaponByCRC(int, ISceneNode *, int);
};
extern CWeaponFactory g_WeaponFactory;

int CTankObject::AddWeapon(int slot, int crc) {
    if (crc == (int)0xfb9630c9) {
        weapons[slot] = 0;
        return 0;
    }
    CWeapon *weapon = g_WeaponFactory.CreateWeaponByCRC(crc, (ISceneNode *)this, 999);
    weapons[slot] = weapon;
    if (weapon) {
        if (weapon->properties->projectile)
            weapon->bulletClass = 4;
        else
            weapon->bulletClass = 10;
        return 1;
    }
    return 0;
}
