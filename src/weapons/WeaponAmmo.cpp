// AI-assisted reconstruction from GR8E69; see docs/Weapons.md.
#include "Weapon.h"
#include "AIBehavior.h"
#include "Toilet.h"
class CDrawContext;
struct BSGO_Basic;
void DestroyBSObject(BSObject *);
void ForceUpdateProximityTriggerStatus(BSGO_Basic *, bool);
class CBulletFactory {
  public:
    void *GetBulletSprite(bool, int);
};
extern CBulletFactory *g_pBulletFactory;
void CWeapon::RegisterWithAI() {
    if (script) {
        // Record 3 of the resource found for this weapon holds the behaviour CRC.
        int behaviorCRC = TLT_FindNextResourceByTypeAndVal((TLTResourceID)20, 1, crc, 0)->data[3]->value;
        CAIObject *ai = ((ISceneNode *)holder->AsSoldierObject())->GetAIObject();
        ((CPistolBehavior *)ai->FindMatchingBehaviorByCRC(behaviorCRC))->SetWeaponScript(script);
    }
}
int CWeapon::SupportsCoverpointBehavior() {
    switch (weaponClass) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        return 1;
    default:
        return 0;
    }
}
int CWeapon::SupportsGrenadeResponseBehavior() {
    switch (weaponClass) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        return 1;
    default:
        return 0;
    }
}
void CWeapon::Shutdown() {
    if (script) {
        DestroyBSObject(script);
        script = 0;
    }
}
void *CWeapon::GetBulletSprite() const {
    return g_pBulletFactory->GetBulletSprite(properties->projectile != 0, properties->bulletIndex);
}
void CWeapon::Shoot(EWeaponShootType type, float value, float value2) {
    shootType = type;
    shootValue = value;
    if (value2 != -1.0f) shootValue2 = value2;
}
void CWeapon::AddAmo(short count) {
    if (reserveAmmo + loadedAmmo == 0) reloading = 1;
    reserveAmmo += count;
    if (reserveAmmo > properties->infoValue1) reserveAmmo = properties->infoValue1;
    if (weaponClass == 7 && reserveAmmo == properties->infoValue1) reserveAmmo--;
}
void CWeapon::SetAmmo(unsigned int loaded, unsigned int reserve) {
    loadedAmmo = loaded;
    reserveAmmo = reserve;
}
void CWeapon::Empty() {
    loadedAmmo = reserveAmmo = 0;
}
void CWeapon::StartReloading() { reloading = 1; }
void CWeapon::ReloadSingle() {
    if (loadedAmmo < properties->clipSize && reserveAmmo > 0) {
        loadedAmmo++;
        reserveAmmo--;
    }
}
void CWeapon::DoneReloading() {
    reloading = 0;
    int infinite = infiniteAmmo;
    if (infinite) {
        loadedAmmo = properties->clipSize;
    } else if (loadedAmmo + reserveAmmo < properties->clipSize) {
        loadedAmmo += reserveAmmo;
        reserveAmmo = 0;
    } else {
        reserveAmmo -= properties->clipSize - loadedAmmo;
        loadedAmmo = properties->clipSize;
    }
}
void CWeapon::BeginUpdate(float elapsed) {
    if (forceProximityUpdate) ForceUpdateProximityTriggerStatus((BSGO_Basic *)&trigger, true);
    CStaticObject::BeginUpdate(elapsed);
}
void CWeapon::Draw(CDrawContext &context) {
    if (model && !holder->AsPlayerObject()) CStaticObject::Draw(context);
}
