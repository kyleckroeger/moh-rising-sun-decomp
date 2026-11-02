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
void CThrownBullet::OnCollision(const CCollision &collision) {
    ISceneNode *other = (ISceneNode *)(collision.first != (void *)this ? collision.first : collision.second);
    if (other->GetCollisionId() == 8) {
        g_scene.ExcludeCollision(*this, *other, -1);
        return;
    }
    if (noCatch) return;
    if (explodeNow) return;
    if (!(collision.depth >= field_b4)) {
        CVector3 normal(collision.contacts->normal);
        if (!(Dot(normal, velocity) <= 0.0f)) Negate(normal);
        field_160 = normal;
        bit30 = 1;
        field_b4 = collision.depth;
    }
    switch (type) {
    case 4:
        bit29 = 1;
        break;
    case 0:
    case 6:
    case 8:
    case 9:
    case 10:
    case 11:
        if (!bit30) return;
        Explode();
        break;
    default:
        DebugMsg("WARNING: Unknown thrown bullet type in OnCollision.  Type %d\n", type);
        break;
    case 1:
    case 2:
    case 5:
    case 7:
        break;
    }
    if (bit30) {
        ISceneNode *hit = (ISceneNode *)(collision.first != (void *)this ? collision.first : collision.second);
        hit->HandleBulletCollision(this, collision);
    }
}
