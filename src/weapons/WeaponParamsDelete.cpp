// AI-assisted reconstruction from GR8E69; see docs/Weapons.md.
#include "profile.h"
#include <map>
#include "Weapon.h"
class CWeaponFactory {
  public:
    _STL::map<int, WeaponProperties_struct *> properties; // +0x00
    void DeleteWeaponParameters();
};
void CWeaponFactory::DeleteWeaponParameters() {
    _STL::map<int, WeaponProperties_struct *>::iterator it = properties.begin();
    for (; it != properties.end(); ++it) {
        delete (*it).second;
        (*it).second = 0;
    }
}
