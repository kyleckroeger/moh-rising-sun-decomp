// AI-assisted scoped reconstruction from GR8E69; see docs/Script.md.
#ifndef GAME_WEAPON_H
#define GAME_WEAPON_H
#include "CVector3.h"
#include "SceneObjects.h"

// Member-only view of CWeapon for the reload built-ins. This does not establish
// the complete allocation, base classes or virtual interface.
// Observed fields of the weapon's property record; not a complete layout.
class CSprite;
struct WeaponProperties_struct {
    int crc;          // +0x00; passed to CreateDumpObject by DropMPDumpObject.
    short clipSize;   // +0x04; compared by BIFunc_PlayerWeaponInfo selector 4.
    short refireRate; // +0x06
    float bulletSpeedScale; // +0x08; scales the bullet weight in GetBulletSpeed.
    short bulletIndex; // +0x0c; index into the bullet factory's tables.
    short projectile; // +0x0e; nonzero for projectile weapons.
    unsigned char unknown_10[2];
    short infoValue1; // +0x12; returned for EWeaponInfo 1; caps the reserve in AddAmo.
    unsigned char unknown_14[0x34 - 0x14];
    void *model;      // +0x34; copied to the static object by Init.
    char iconName[0x58 - 0x38]; // +0x38; loaded by LoadWeaponGUISprites.
    CSprite *icon;    // +0x58
    unsigned char unknown_5c[0x60 - 0x5c];
    short soundType;  // +0x60; returned by GetWeaponSoundType.
    short field_62;   // Nonzero selects alternate grenade-catching side bounds.
    unsigned char unknown_64[0x68 - 0x64];
    short weaponType; // +0x68; returned by the weapon-type built-ins.
};

typedef WeaponProperties_struct WeaponProperties;

// Original enum tag; its members are unknown.
enum EWeaponShootType { WeaponShootTypeUnknown = 0 };

// Original enum tag; selector names are descriptive.
enum EWeaponInfo { WeaponInfo0 = 0, WeaponInfo1 = 1, WeaponInfo2 = 2 };

// Direct base call targets establish CStaticObject as the base; further bases and
// the virtual interface are not declared.
class CWeapon : public CStaticObject {
  public:
    unsigned char unknown_220[0x300 - sizeof(CStaticObject)];
    WeaponProperties *properties; // +0x300
    void *state;      // +0x304; a Resource::Pointer<EALA::Character::State>.
    int crc;          // +0x308; weapon name CRC.
    short reserveAmmo; // +0x30c
    short loadedAmmo; // +0x30e; rounds in the clip, returned for EWeaponInfo 0.
    float field_310;            // +0x310
    EWeaponShootType shootType; // +0x314
    float shootValue;           // +0x318
    int bulletClass;            // +0x31c; chosen from the holder's collision id.
    int weaponClass; // +0x320; returned by BIFunc_PlayerGetWeaponClass.
    float shootValue2;          // +0x324; Shoot keeps it when passed -1.
    unsigned char unknown_328[0x340 - 0x328];
    CVector3 position;          // +0x340; returned by GetPosition.
    unsigned int reloading : 1; // +0x350, bit 31: cleared by BIFunc_PlayerClearReloading.
    unsigned int field_30 : 1;
    unsigned int forceProximityUpdate : 1; // Bit 29; tested by BeginUpdate.
    unsigned int infiniteAmmo : 1;         // Bit 28; a reload fills the clip without using the reserve.
    unsigned int field_27 : 1;
    unsigned int muzzleFlashStatus : 1;    // Bit 26; returned by GetMuzzleFlashStatus.
    unsigned int muzzlePuff : 1;           // Bit 25; CreateMuzzlePuff does nothing while clear.
    unsigned int field_24 : 1;
    unsigned int unknown_low : 24;
    float field_354;            // +0x354
    float field_358;            // +0x358
    float field_35c;            // +0x35c; a random angle while the muzzle flash shows.
    void *muzzleFlash; // +0x360
    float field_364;            // +0x364; from the level resource record.
    int field_368;              // +0x368; from the level resource record.
    float field_36c;            // +0x36c
    ISceneNode *holder; // +0x370
    int trigger;        // +0x374; proximity-trigger subobject, only its address is taken here.
    void Init(int, WeaponProperties_struct *, ISceneNode *, short);
    void RegisterWithAI();
    int SupportsCoverpointBehavior();
    int SupportsGrenadeResponseBehavior();
    void Shutdown();
    void *GetBulletSprite() const;
    void AddAmo(short);
    void FireBullet();
    void SetAmmo(unsigned int, unsigned int);
    void Empty();
    void BeginUpdate(float);
    void Draw(CDrawContext &);
    float GetBulletSpeed() const;
    int GetWeaponSoundType() const;
    void SetInfiniteAmo(bool);
    void Update(float, bool);
    void ConfigMuzzleFlash();
    unsigned int GetMuzzleFlashStatus();
    void CreateMuzzlePuff(int, const CMatrix &);
    CWeapon *AsWeaponObject();
    void GetPosition(CVector3 &) const;
    void StartReloading();
    void ReloadSingle();
    void DoneReloading();
    void Shoot(EWeaponShootType, float, float);
};

int GetWeaponInfoHelper(CWeapon *, EWeaponInfo);

#endif
