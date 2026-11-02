// AI-assisted reconstruction from GR8E69; see docs/Weapons.md.
#include "Weapon.h"
#include "IDestructible.h"
class CWeaponFactory {
  public:
    void DestroyWeapon(CWeapon *);
};
void CWeaponFactory::DestroyWeapon(CWeapon *weapon) {
    weapon->Shutdown();
    ((IDestructible *)weapon)->MarkForDestruction(1);
}
