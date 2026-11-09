// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original anim_obj.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "Bullet.h"
#include "CameraShake.h"
#include "GameSettings.h"
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

void PlayImpactSound(unsigned short, unsigned short, const CVector3 &, bool);

static inline float Dot(const CVector3 &a, const CVector3 &b) { return a.x * b.x + a.y * b.y + a.z * b.z; }


void CAnimObject::HandleBulletCollision(CBullet *bullet, const CCollision &collision) {
    CProjectileBullet *projectile = bullet->AsProjectile();
    CThrownBullet *thrown = bullet->AsThrown();
    if (projectile) {
        BS_STRUCT_HitByBulletInfo_struct *info = (BS_STRUCT_HitByBulletInfo_struct *)BSEventGetEventMemoryBlock(32);
        info->bulletType = bullet->GetScriptBulletType();
        info->damage = bullet->GetDamage();
        ISceneNode *firer = bullet->GetFiredBy();
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
        switch (info->location) {
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
            info->damage *= 0.5f;
            break;
        case 0:
        case 1:
            info->damage *= 4.0f;
            break;
        case 2:
        case 9:
        case 10:
        case 11:
        case 12:
            break;
        }
        BSObjectTriggerEvent(script, 40, info, 0, 0);
        if (firer->AsPlayerObject() && !(firer->AsPlayerObject()->pathFlags & 0x8000)) {
            g_GameSettings.Set_hits(1, 0);
            switch (info->location) {
            case 3:
            case 4:
            case 5:
                g_GameSettings.Set_shotLocation(1, (EShotLocations)1, 0);
                break;
            case 6:
            case 7:
            case 8:
                g_GameSettings.Set_shotLocation(1, (EShotLocations)2, 0);
                break;
            case 13:
            case 14:
            case 15:
                g_GameSettings.Set_shotLocation(1, (EShotLocations)4, 0);
                break;
            case 16:
            case 17:
            case 18:
                g_GameSettings.Set_shotLocation(1, (EShotLocations)5, 0);
                break;
            case 19:
            case 20:
                g_GameSettings.Set_shotLocation(1, (EShotLocations)6, 0);
                break;
            case 0:
            case 1:
                g_GameSettings.Set_shotLocation(1, (EShotLocations)0, 0);
                break;
            case 2:
            case 9:
            case 10:
            case 11:
            case 12:
                g_GameSettings.Set_shotLocation(1, (EShotLocations)3, 0);
                break;
            }
        }
        PlayImpactSound(41, 1, contact->position, 0);
    } else if (thrown) {
        thrown->Explode();
    }
}
