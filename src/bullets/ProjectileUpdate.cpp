// AI-assisted reconstruction from GR8E69; see docs/Bullets.md.
#include "Bullet.h"
#include "Fiber.h"
#include "Volume.h"
#define FIBER(p) ((CVolFiber *)(p))
static inline void SetScaled(CVector3 &r, const CVector3 &v, float s) { r.x = v.x * s; r.y = v.y * s; r.z = v.z * s; }
static inline void SetSum(CVector3 &r, const CVector3 &a, const CVector3 &b) { r.x = a.x + b.x; r.y = a.y + b.y; r.z = a.z + b.z; }
void CProjectileBullet::BeginUpdate(float dt) {
    if (flags.hit) return;
    if (collisionTime != -1.0f) {
        life += dt - collisionTime;
        collisionTime = -1.0f;
    }
    life -= dt;
    position = localToWorld.GetPos();
    prevPosition = position;
    field_b4 = 3.4028235e+38f;
    if (!(field_b0 <= 0.0f)) {
        field_b0 -= dt;
        if (!(field_b0 >= 0.0f)) field_b0 = 0.0f;
    }
}
void CProjectileBullet::AttemptUpdate(float dt) {
    if (flags.hit) return;
    field_b4 = 3.4028235e+38f;
    float step = speed * dt;
    CVector3 offset;
    SetScaled(offset, localToWorld.GetFront(), step);
    CVector3 next;
    SetSum(next, position, offset);
    localToWorld.SetPos(next);
    CVector3 start(0.0f, -step, 0.0f);
    FIBER(localVolume)->Set(start, CVector3(0.0f, 0.0f, 0.0f));
    FIBER(worldVolume)->CVolFiber::TransformedCopy(*FIBER(localVolume), localToWorld);
    CVector3 low, high;
    FIBER(worldVolume)->CVolFiber::GetExtents(low, high);
    extentMinX->value = low.x;
    extentMinY->value = low.y;
    extentMaxX->value = high.x;
    extentMaxY->value = high.y;
}
void CProjectileBullet::CommitUpdate() {
    if (flags.hit) {
        if (!flags.hitPending) {
            flags.hitPending = 1;
            return;
        }
    }
    if (field_b0 <= 0.0f || flags.hitPending) {
        ((ISubject *)this)->MarkForDestruction(1);
    } else {
        position = localToWorld.GetPos();
    }
}
static inline float Dot(const CVector3 &a, const CVector3 &b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
void CProjectileBullet::Penetrate(const CCollision &collision) {
    const CVector3 &normal = *(const CVector3 *)collision.contacts;
    const float eps = 0.0001f;
    CVector3 dir;
    GetForward(dir);
    float dot = Dot(dir, normal);
    if (!((float)__builtin_fabs(dot) <= eps)) {
        dir *= eps / (float)__builtin_fabs(dot);
    } else if (!(dot <= 0.0f)) {
        SetScaled(dir, normal, eps);
    } else {
        SetScaled(dir, normal, -0.0001f);
    }
    CVector3 next;
    SetSum(next, localToWorld.GetPos(), dir);
    localToWorld.SetPos(next);
    flags.hitPending = 0;
}
