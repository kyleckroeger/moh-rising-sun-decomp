// AI-assisted reconstruction from GR8E69; see docs/Bullets.md.
#include "Bullet.h"

// Descriptive inline helpers, preserving the observed component operations.
static inline void SetScaled(CVector3 &r, const CVector3 &v, float s) {
    r.x = v.x * s;
    r.y = v.y * s;
    r.z = v.z * s;
}
static inline void SetSum(CVector3 &r, const CVector3 &a, const CVector3 &b) {
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    r.z = a.z + b.z;
}
static inline float Dot(const CVector3 &a, const CVector3 &b) {
    float sum = a.x * b.x + a.y * b.y;
    sum += a.z * b.z;
    return sum;
}

void CThrownBullet::Penetrate(const CCollision &) {
    const float eps = 0.02f;
    CVector3 normal(field_160);
    CVector3 offset;
    GetForward(offset);
    float dot = Dot(offset, normal);
    if (!(dot <= 0.0f))
        SetScaled(offset, normal, eps);
    else
        SetScaled(offset, normal, -0.02f);
    CVector3 next;
    SetSum(next, localToWorld.GetPos(), offset);
    localToWorld.SetPos(next);
    position = next;
    bit30 = 0;
    explodeNow = 0;
}
