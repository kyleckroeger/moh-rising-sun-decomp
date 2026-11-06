// AI-assisted reconstruction from GR8E69; see docs/Bullets.md.
#include "Bullet.h"
#include "Fiber.h"
#include "GameSettings.h"
extern "C" float sqrtf(float);
#define FIBER(p) ((CVolFiber *)(p))
static inline void SetScaled(CVector3 &r, const CVector3 &v, float s) { r.x = v.x * s; r.y = v.y * s; r.z = v.z * s; }
static inline void SetDiff(CVector3 &r, const CVector3 &a, const CVector3 &b) { r.x = a.x - b.x; r.y = a.y - b.y; r.z = a.z - b.z; }
static inline void Negate(CVector3 &v) {
    v.x = -v.x;
    v.y = -v.y;
    v.z = -v.z;
}
static inline float Dot(const CVector3 &a, const CVector3 &b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
static inline float SumOfSquares(const CVector3 &v) {
    float sum = v.x * v.x + v.y * v.y;
    sum += v.z * v.z;
    return sum;
}
int CThrownBullet::Constrain(float) {
    if (!bit30) return 1;
    float speed = 1.0f;
    if (g_cheats.keepGrenadeSpeed) speed = sqrtf(velocity.LengthSquared());
    CVector3 normal(field_160);
    const CVector3 &n = normal;
    if (!(n.z <= 0.7f)) bit31 = 1;
    float dot = Dot(velocity, normal);
    if (!(dot <= 0.0f)) {
        Negate(field_160);
        normal = field_160;
        dot = Dot(velocity, normal);
    }
    normal *= dot + dot;
    velocity -= normal;
    CVector3 along, across;
    float d = Dot(field_160, velocity);
    SetScaled(along, field_160, d);
    SetDiff(across, velocity, along);
    along *= properties->bounce;
    if (!(SumOfSquares(along) >= 0.0001f) && bit31) along.Zero();
    CVector3 up(0.0f, 0.0f, 1.0f);
    float steep = Dot(up, field_160);
    if (!(steep <= 0.0f)) {
        float cube = steep * steep;
        cube *= steep;
        steep = cube;
    }
    else steep = 0.0f;
    across *= 1.0f - steep * properties->friction;
    along += across;
    velocity = along;
    if (g_cheats.keepGrenadeSpeed) {
        float now = sqrtf(velocity.LengthSquared());
        if (!(now <= 0.01f) && !(speed <= 0.01f)) velocity *= speed / now;
    }
    if (!(field_b4 < 0.001f) && !(field_b4 > 1.0f)) {
        CVector3 result;
        CVector3 end = FIBER(worldVolume)->GetEnd();
        CVector3 start = FIBER(worldVolume)->GetStart();
        result.x = (end.x - start.x) * field_b4 + start.x;
        result.y = (end.y - start.y) * field_b4 + start.y;
        result.z = (end.z - start.z) * field_b4 + start.z;
        localToWorld.SetPos(result);
    }
    spin = velocity;
    return 1;
}
