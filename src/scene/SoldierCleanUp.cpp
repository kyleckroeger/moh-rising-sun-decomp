// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original Soldier_object.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "Volume.h"
#include "AIDoodad.h"
#include "Weapon.h"

class CWeaponFactory {
  public:
    void DestroyWeapon(CWeapon *);
};
extern CWeaponFactory g_WeaponFactory;
class CStaticObjectFactory {
  public:
    static void DeallocateStaticObject(CStaticObject *);
};
class CCharacterShadow {
  public:
    void Release();
};
extern void FreeBoxVolume(void *);


void CSoldierObject::CleanUp() {
    CAnimObject *self = (CAnimObject *)this;
    if (self->localVolumeType0) {
        delete self->localVolumeType0;
        self->localVolumeType0 = 0;
    }
    if (self->worldVolumeType0) {
        delete self->worldVolumeType0;
        self->worldVolumeType0 = 0;
    }
    if (self->localVolumeType3) {
        FreeBoxVolume(self->localVolumeType3);
        self->localVolumeType3 = 0;
    }
    if (self->worldVolumeType3) {
        FreeBoxVolume(self->worldVolumeType3);
        self->worldVolumeType3 = 0;
    }
    RemoveWeapons();
    int slot = GetAttachPoint("head");
    if (slot != -1) {
        // The original forms this + slot * 112 first; the source form of the address is inferred.
        CSoldierObject *base = (CSoldierObject *)((char *)this + slot * 112);
        if (base->attachSlots[0].active && !base->attachSlots[0].keepObject)
            CStaticObjectFactory::DeallocateStaticObject(base->attachSlots[0].object);
    }
    ((CAISoldierDoodad *)aiDoodad)->CleanUp();
    ((CCharacterShadow *)shadowStorage)->Release();
}

