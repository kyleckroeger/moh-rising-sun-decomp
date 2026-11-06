// AI-assisted reconstruction from GR8E69; see docs/Weapons.md.
#include "Weapon.h"
#include "ParticleClock.h"
#include "ParticleRecipe.h"
class CBulletFactory {
  public:
    float GetBulletWeight(bool, int);
};
extern CBulletFactory *g_pBulletFactory;
extern void *g_pStaticObjectMuzzleFlashNPC;
float CWeapon::GetBulletSpeed() const {
    return g_pBulletFactory->GetBulletWeight(properties->projectile != 0, properties->bulletIndex) * properties->bulletSpeedScale;
}
int CWeapon::GetWeaponSoundType() const { return properties->soundType; }
void CWeapon::SetInfiniteAmo(bool infinite) { infiniteAmmo = infinite; }
void CWeapon::ConfigMuzzleFlash() { muzzleFlash = g_pStaticObjectMuzzleFlashNPC; }
unsigned int CWeapon::GetMuzzleFlashStatus() { return muzzleFlashStatus; }
void CWeapon::CreateMuzzlePuff(int id, const CMatrix &m) {
    if (muzzlePuff) {
        IParticleSystem *puff = CPSManager::Create(id, true, eParticleSystemDefault);
        if (puff) puff->SetTMLocalToWorld(m);
    }
}
CWeapon *CWeapon::AsWeaponObject() { return this; }
void CWeapon::GetPosition(CVector3 &v) const { v = position; }
