// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
#include "CameraShake.h"
#include "Camera.h"
#define E(i, j) (a.row[i].x * b.row[0].j + a.row[i].y * b.row[1].j + a.row[i].z * b.row[2].j + a.row[i].w * b.row[3].j)
static inline void Concat(CMatrix &r, const CMatrix &a, const CMatrix &b) {
    r.row[0].x = E(0, x); r.row[0].y = E(0, y); r.row[0].z = E(0, z); r.row[0].w = E(0, w);
    r.row[1].x = E(1, x); r.row[1].y = E(1, y); r.row[1].z = E(1, z); r.row[1].w = E(1, w);
    r.row[2].x = E(2, x); r.row[2].y = E(2, y); r.row[2].z = E(2, z); r.row[2].w = E(2, w);
    r.row[3].x = E(3, x); r.row[3].y = E(3, y); r.row[3].z = E(3, z); r.row[3].w = E(3, w);
}
#undef E
static inline void SetVector(CVector3 &to, const CVector3 &from) { to = from; }
static inline void CopyMatrix(CMatrix &to, const CMatrix &from) { to = from; }
static inline void PreMultiply(CMatrix &m, const CMatrix &o) {
    CMatrix::s_TempMat = m;
    m.Multiply(o, CMatrix::s_TempMat);
}
void CPlayerObject::SetPlayersMountedMGPositions(CMatrix *matrix) {
    localToWorld = *matrix;
    PreMultiply(localToWorld, mountedPlayerOffset);
    CMatrix world;
    CMatrix *w = &world;
    if (!CMatrix::s_ClassInit) CMatrix::InitClass();
    const CMatrix *offset = &mountedOffset;
    Concat(*w, cameraMatrix, localToWorld);
    ((CCamera *)&clip)->CCamera::SetTMLocalToWorld(*w);
    ((CCamera *)&clip)->CCamera::AttemptUpdate(1.0f);
    CMatrix gun;
    CopyMatrix(gun, *matrix);
    if (flags.bit18) {
        if (flags.bit17) offset = &mountedOffsetAlt;
    }
    PreMultiply(gun, *offset);
    SetVector(mountedWeapon->direction, gun.GetFront());
    SetVector(mountedWeapon->position, gun.GetPos());
}
