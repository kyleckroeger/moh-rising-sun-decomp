// AI-assisted reconstruction from GR8E69; see docs/Weapons.md.
#include "CameraShake.h"
class CWeaponFactory {
  public:
    void DestroyWeapon(CWeapon *);
};
extern CWeaponFactory g_WeaponFactory;
void CPlayerWeaponObject::Shutdown() {
    if (weapon) g_WeaponFactory.DestroyWeapon(weapon);
    g_scene.Remove(*(ISceneNode *)this);
}
