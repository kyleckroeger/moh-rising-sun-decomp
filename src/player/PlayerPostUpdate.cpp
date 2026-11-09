// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
#include "CameraShake.h"
static inline void CopyMatrix(CMatrix &to, const CMatrix &from) { to = from; }
static inline void SetVector(CVector3 &to, const CVector3 &from) { to = from; }
static inline void PreMultiply(CMatrix &m, const CMatrix &o) {
    CMatrix::s_TempMat = m;
    m.Multiply(o, CMatrix::s_TempMat);
}
void CPlayerObject::PostUpdate(float dt) {
    if (mountedWeapon) {
        if (pathFlags & 0x1000) {
            CMatrix gun;
            CMatrix *m = &gun;
            if (!CMatrix::s_ClassInit) CMatrix::InitClass();
            ((ISceneNode *)mountedGun)->GetTMLocalToWorld(*m);
            const CMatrix *offset = &mountedOffset;
            if (flags.bit18) {
                if (flags.bit17) offset = &mountedOffsetAlt;
            }
            PreMultiply(*m, *offset);
            SetVector(mountedWeapon->direction, gun.GetFront());
            SetVector(mountedWeapon->position, gun.GetPos());
        }
        unsigned int path = pathFlags;
        CWeapon *weapon = mountedWeapon;
        bool active = false;
        if (!(path & 0x8000)) {
            if (path & 0x08000000) active = false;
            else active = true;
        }
        weapon->Update(dt, active);
        if (mountedWeapon->field_30) {
            if (flags.bit18) {
                char value = mountedShotSide;
                int second = flags.bit17;
                char side = value | 1;
                if (second) side = value | 2;
                flags.bit17 = second ^ 1;
                mountedShotSide = side;
            }
            if (flags.bit19) EjectMountedMGShell();
        }
    }
}
