// AI-assisted reconstruction from GR8E69; see docs/Weapons.md.
extern void *g_pStaticObjectMuzzleFlashPlayer;
#include "CameraShake.h"
void CPlayerWeaponObject::DoneReloading() { weapon->DoneReloading(); }
void CPlayerWeaponObject::AddAmo(int count) { weapon->AddAmo(count); }
void CPlayerWeaponObject::SetAmmo(unsigned int loaded, unsigned int reserve) { weapon->SetAmmo(loaded, reserve); }
unsigned int CPlayerWeaponObject::IsDrawEnabled() const { return (flags & 0x40000000) && !player->flags.hideWeapon; }
int CPlayerWeaponObject::GetBulletEmitterMatrix() {
    WeaponAnimOwner *owner = &animOwner;
    muzzleJoint = -1;
    muzzleJoint = owner->GetSkeleton()->FindJoint("jo_fire");
    flags |= 0x10000000;
    muzzleFlash = g_pStaticObjectMuzzleFlashPlayer;
    return 1;
}
