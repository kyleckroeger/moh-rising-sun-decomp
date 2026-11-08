// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original Tank_object.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "Weapon.h"
#include "CMatrix.h"

static inline void PostMultiply(CMatrix &m, const CMatrix &o) {
    CMatrix::s_TempMat = m;
    m.Multiply(o, CMatrix::s_TempMat);
}
static inline void AimWeapon(CWeapon *w, const CVector3 &d) { w->direction = d; }
static inline void PlaceWeapon(CWeapon *w, const CVector3 &p) { w->position = p; }

void CTankObject::FireSubObject(int subObject) {
    CMatrix transform;
    CMatrix *matrix = &transform;
    if (!CMatrix::s_ClassInit)
        CMatrix::InitClass();
    CMatrix attach;
    if (!CMatrix::s_ClassInit)
        CMatrix::InitClass();
    int slot = weaponSlots[subObject];
    if (slot >= 0 && slot < 3) {
        CHierObject *sub = (CHierObject *)GetSubObject(subObject);
        weapons[slot]->muzzleFlashStatus = 1;
        *matrix = sub->transform;
        sub->GetAttachPointMatrix(0xe6e804d5, attach);
        PostMultiply(*matrix, attach);
        AimWeapon(weapons[slot], transform.GetFront());
        PlaceWeapon(weapons[slot], transform.GetPos());
        weapons[slot]->Shoot((EWeaponShootType)3, 1.0f, -1.0f);
    }
}
