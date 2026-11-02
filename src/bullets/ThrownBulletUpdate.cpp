// AI-assisted reconstruction from GR8E69; see docs/Bullets.md.
#include "Bullet.h"
#include "Fiber.h"
#include "IDestructible.h"
extern "C" float sqrtf(float);
#define FIBER(p) ((CVolFiber *)(p))
static inline void SetScaled(CVector3 &r, const CVector3 &v, float s) { r.x = v.x * s; r.y = v.y * s; r.z = v.z * s; }
static inline void SetSum(CVector3 &r, const CVector3 &a, const CVector3 &b) { r.x = a.x + b.x; r.y = a.y + b.y; r.z = a.z + b.z; }
static inline float Dot(const CVector3 &a, const CVector3 &b) {
    float sum = a.x * b.x + a.y * b.y;
    sum += a.z * b.z;
    return sum;
}
static inline void Normalize(CVector3 &v) {
    float length = sqrtf(v.LengthSquared());
    if (length != 0.0f) v *= 1.0f / length;
}
static inline void SetCross(CVector3 &out, const CVector3 &a, const CVector3 &b) {
    CVector3 r;
    r.x = a.y * b.z - a.z * b.y;
    r.y = a.z * b.x - a.x * b.z;
    r.z = a.x * b.y - a.y * b.x;
    out.x = r.x;
    out.y = r.y;
    out.z = r.z;
}
class BSGO_Basic;
void ForceUpdateProximityTriggerStatus(BSGO_Basic *, bool);
void DebugMsg(const char *, ...);
class CGrenadeResponseBehavior {
  public:
    static void UpdateGrenade(CThrownBullet *);
    static void GrenadeDestroyed(CThrownBullet *);
};
static inline void Negate(CVector3 &v) {
    v.x = -v.x;
    v.y = -v.y;
    v.z = -v.z;
}
void CThrownBullet::BeginUpdate(float dt) {
    lightVolumes.Update(dt);
    if (type == 1 || type == 2) CGrenadeResponseBehavior::UpdateGrenade(this);
    position = localToWorld.GetPos();
    switch (type) {
    case 1:
    case 2:
    case 4:
    case 5:
    case 7:
        if (!bit31 && !(sqrtf(velocity.LengthSquared()) <= 0.001f)) {
            CVector3 front(velocity);
            CVector3 right, up;
            Normalize(front);
            ((ISceneNode *)this)->GetRightward(right);
            SetCross(up, right, front);
            Normalize(up);
            SetCross(right, front, up);
            Normalize(right);
            localToWorld.SetFront(front);
            localToWorld.SetRight(right);
            localToWorld.SetUp(up);
        }
    case 0:
    case 6:
    case 8:
    case 9:
    case 10:
    case 11:
        break;
    default:
        DebugMsg("WARNING: Unknown thrown bullet type in BeginUpdate.  Type %d\n", type);
        break;
    }
    spin = velocity;
    field_180 = field_170;
    ForceUpdateProximityTriggerStatus((BSGO_Basic *)scriptNode, true);
    if (!(field_b0 <= 0.0f)) {
        field_b0 -= dt;
        if (!(field_b0 >= 0.0f)) field_b0 = 0.0f;
    }
    field_b4 = 3.4028235e+38f;
}
void CThrownBullet::AttemptUpdate(float dt) {
    if (noCatch) return;
    field_b4 = 3.4028235e+38f;
    velocity = spin;
    if (bit29) velocity.z = dt * -0.002727675484f + velocity.z;
    CVector3 step;
    SetScaled(step, velocity, dt);
    float length = sqrtf(step.LengthSquared());
    SetScaled(field_170, field_190, length);
    field_170 += field_180;
    step += position;
    localToWorld.SetPos(step);
    FIBER(worldVolume)->Set(position, step);
    CVector3 low, high;
    FIBER(worldVolume)->CVolFiber::GetExtents(low, high);
    extentMinX->value = low.x;
    extentMinY->value = low.y;
    extentMaxX->value = high.x;
    extentMaxY->value = high.y;
}
