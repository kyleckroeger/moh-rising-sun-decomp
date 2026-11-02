// AI-assisted reconstruction from GR8E69; see docs/Weapons.md.
#include "CameraShake.h"
#include <string.h>
void CPlayerWeaponObject::SetAnimFolder(const char *folder) { strcpy(name, folder); }
void CPlayerWeaponObject::SetWeaponName(const char *value) {
    if (value) strcpy(weaponName, value);
    else weaponName[0] = 0;
}
void SetMuzzleStatusForPlayer(bool, int player) {
    g_scene.GetPlayer(player)->GetCurrentWeapon()->muzzleFlashStatus = 1;
}
class CAnimObjectFactory {
  public:
    void ReleaseObject(CAnimObject *);
};
extern CAnimObjectFactory g_AnimObjFactory;
void CPlayerWeaponObject::Destroy() { g_AnimObjFactory.ReleaseObject((CAnimObject *)this); }
