// AI-assisted reconstruction from GR8E69; see docs/Weapons.md.
#include "CameraShake.h"
void CPlayerWeaponObject::Init() { ((CAnimObject *)this)->Init(); }
void CPlayerWeaponObject::SetDrawDelay(int delay) { state = delay; }
CPlayerWeaponObject *CPlayerWeaponObject::AsPlayerWeaponObject() { return this; }
const CPlayerWeaponObject *CPlayerWeaponObject::AsPlayerWeaponObject() const { return this; }
