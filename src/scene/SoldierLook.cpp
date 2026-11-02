// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original Soldier_object.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "Weapon.h"

class CWeaponFactory {
  public:
    void DestroyWeapon(CWeapon *);
    CWeapon *CreateWeaponByCRC(int, ISceneNode *, int);
};
extern CWeaponFactory g_WeaponFactory;
namespace EALA {
namespace Character {
class Skeleton {
  public:
    const CMatrix *FindAttachPoint(const char *);
};
}
}

void CSoldierObject::SetAttachedLight(CLight *light, const CVector3 &offset) {
    if (light) {
        CMatrix matrix;
        CMatrix *saved = &matrix;
        if (!CMatrix::s_ClassInit)
            CMatrix::InitClass();
        ((ISceneNode *)this)->GetTMLocalToWorld(*saved);
        saved->PreTranslate(offset);
        ((IMovingSceneNode *)light)->SetTMLocalToWorld(*saved);
    }
}

int CSoldierObject::AddWeapon(int slot, int crc, int ammo) {
    if (weapons[slot])
        g_WeaponFactory.DestroyWeapon(weapons[slot]);
    if (crc == (int)0xfb9630c9) {
        weapons[slot] = 0;
        return 1;
    }
    weapons[slot] = g_WeaponFactory.CreateWeaponByCRC(crc, (ISceneNode *)this, ammo);
    if (weapons[slot])
        return 1;
    return 0;
}

void CSoldierObject::GetLookMatrix(CMatrix &out) {
    int attach = GetAttachPoint("VisionCone");
    out = ((CAnimObject *)this)->transform;
    if (attach != -1) {
        const CMatrix *point = ((EALA::Character::Skeleton *)((CAnimObject *)this)->skeleton)->FindAttachPoint("VisionCone");
        if (point) {
            CMatrix::s_TempMat = out;
            out.Multiply(*point, CMatrix::s_TempMat);
        }
    }
}
