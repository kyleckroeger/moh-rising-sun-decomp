// AI-assisted reconstruction from GR8E69; see docs/Bullets.md.
#include "GrenadeResponse.h"
#include "Bullet.h"
#include "SceneObjects.h"
#include "AITargeting.h"
#include "Weapon.h"

extern "C" float sqrtf(float);

static inline void SetDiff(CVector3 &r, const CVector3 &a, const CVector3 &b) {
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
}
static inline float Dot(const CVector3 &a, const CVector3 &b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

CVector3 GetVectorToGrenade(CSoldierObject *soldier, const CThrownBullet *grenade) {
    CVector3 position;
    ((ISceneNode *)soldier)->GetPosition(position);
    CVector3 grenadePosition;
    grenade->GetPosition(grenadePosition);
    SetDiff(position, grenadePosition, position);
    return position;
}

bool IsAlive(CSoldierObject *soldier) {
    if (((ISceneNode *)soldier)->GetAIObject()->GetAIStatusVar((aistatus_var)0) <= 0)
        return false;
    return true;
}

bool IsCatchable(CSoldierObject *soldier, CThrownBullet *, const CVector3 &delta, float) {
    if (delta.z <= 1.3f || delta.z >= 1.9f)
        return false;
    CVector3 facing;
    ((ISceneNode *)soldier)->GetForward(facing);
    float projection = Dot(facing, delta);
    if (projection >= 1.0f || projection <= 0.0f)
        return false;
    float low = -0.1f, high = 0.5f;
    if (soldier->GetCurrentWeapon() && soldier->GetCurrentWeapon()->properties->field_62) {
        low = -0.3f;
        high = 0.2f;
    }
    ((ISceneNode *)soldier)->GetRightward(facing);
    projection = Dot(facing, delta);
    if (!(projection >= high) && projection > low)
        return true;
    return false;
}

bool IsStoppedOnGround(CSoldierObject *soldier, const CThrownBullet *grenade) {
    CVector3 delta = GetVectorToGrenade(soldier, grenade);
    return IsStoppedOnGround(delta, sqrtf(grenade->velocity.LengthSquared()));
}

bool IsStoppedOnGround(const CVector3 &delta, float speed) {
    bool result = false;
    if (!(delta.z >= 0.09f))
        result = speed < 0.06f;
    return result;
}

float DistanceSquaredToGrenade(CSoldierObject *soldier, CThrownBullet *grenade) {
    return GetVectorToGrenade(soldier, grenade).LengthSquaredXY();
}

bool InDanger(CSoldierObject *soldier, CThrownBullet *grenade) {
    return DistanceSquaredToGrenade(soldier, grenade) < 36.0f;
}
