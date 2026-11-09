// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original static_obj.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "Bullet.h"
#include "BSGameObject.h"
#include "ScriptRuntime.h"
#include "ScriptTimers.h"
#include "Volume.h"

void *BSEventGetEventMemoryBlock(unsigned int);

// The script event's record; the layout is shared with the player's hit event.
struct BS_STRUCT_HitByBulletInfo_struct {
    BSObject *source;
    float damage;
    int direction;
    int bulletType;
    int location;
    unsigned char unknown_14[4];
    float offsetX; // +0x18; hit position relative to the object.
    float offsetY; // +0x1c
};

static inline float Dot(const CVector3 &a, const CVector3 &b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

static inline bool HasFlag(unsigned int flags, unsigned int mask) { return (flags & mask) != 0; }
static inline int ThrownType(const CThrownBullet *thrown) { return thrown->type; }

void CStaticObject::HandleBulletCollision(CBullet *bullet, const CCollision &collision) {
    CProjectileBullet *projectile = bullet->AsProjectile();
    CThrownBullet *thrown = bullet->AsThrown();
    if (projectile) {
        if (script) {
            ISceneNode *firer = bullet->GetFiredBy();
            if (firer == (ISceneNode *)this) {
                bullet->Penetrate(collision);
            } else {
                if (HasFlag(mechFlags, 1))
                    bullet->Penetrate(collision);
                BS_STRUCT_HitByBulletInfo_struct *info = (BS_STRUCT_HitByBulletInfo_struct *)BSEventGetEventMemoryBlock(32);
                info->bulletType = bullet->GetScriptBulletType();
                info->damage = bullet->GetDamage();
                if (firer)
                    info->source = (BSObject *)firer->GetScriptObject();
                else
                    info->source = (BSObject *)firer;
                CVector3 direction, forward, right;
                const CVector3 &d = direction;
                bullet->GetForward(direction);
                ((ISceneNode *)this)->GetForward(forward);
                ((ISceneNode *)this)->GetRightward(right);
                float front = Dot(forward, d);
                float side = Dot(right, d);
                if (!(side >= -0.7071f))
                    info->direction = 3;
                else if (!(side <= 0.7071f))
                    info->direction = 2;
                else if (!(front >= 0.0f))
                    info->direction = 0;
                else
                    info->direction = 1;
                const SClsnContact *contact = collision.contacts;
                info->location = contact->feature;
                CVector3 position;
                ((ISceneNode *)this)->GetPosition(position);
                info->offsetX = contact->position.x - position.x;
                info->offsetY = contact->position.y - position.y;
                BSObjectTriggerEvent(script, 40, info, 0, 0);
            }
        }
    } else if (thrown) {
        if (thrown->GetFiredBy() == (ISceneNode *)this) {
            thrown->Penetrate(collision);
        } else {
            if (ThrownType(thrown) == 1 || ThrownType(thrown) == 2) {
                if (!((CStaticObjectInterface *)this)->AsHierTankObject())
                    return;
            }
            if (((CStaticObjectInterface *)this)->AsHierTankObject()) {
                BSObject *owner = (BSObject *)((ISceneNode *)((CStaticObjectInterface *)this)->AsHierTankObject())->GetScriptObject();
                if (*(float *)owner->gameObject->GetScriptData() <= 0.0f)
                    return;
            }
            thrown->Explode();
        }
    }
}
