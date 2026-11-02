// AI-assisted reconstruction from GR8E69; see docs/Weapons.md.
#include "Weapon.h"
#include "CDBObject.h"
#include "CLine3.h"
#include "Fiber.h"
#include "GameSettings.h"
#include "Volume.h"
#include "WorldObject.h"
extern "C" float sqrtf(float);
float MathFunRandomReal(float, float);
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
void CWeapon::Update(float dt, bool paused) {
    field_30 = 0;
    if (g_cheats.infiniteAmmo && reserveAmmo == 0) reserveAmmo = 1;
    switch (shootType) {
    case 0:
        if (!(field_310 > 0.0f)) {
            if (loadedAmmo > 0) {
                if (!reloading && !paused) FireBullet();
            } else if (!reloading && !paused) {
                if (holder) holder->TriggerScriptEvent(26, (void *)reserveAmmo, false);
                reloading = 1;
            }
        } else {
            field_310 -= dt;
        }
        if (field_310 > 0.0f) break;
        if (holder && !paused) holder->TriggerScriptEvent(21, 0, true);
        break;
    case 1:
        if (!(field_310 <= 0.0f)) {
            field_310 -= dt;
            if (!(field_310 > 0.0f) && holder && !paused) holder->TriggerScriptEvent(21, 0, true);
        }
        if (loadedAmmo <= 0 && !reloading && !paused) {
            if (holder) holder->TriggerScriptEvent(26, (void *)reserveAmmo, false);
            reloading = 1;
        }
        break;
    case 3:
        shootType = (EWeaponShootType)1;
        if (!paused) FireBullet();
        if (loadedAmmo <= 0 && !paused) {
            if (holder) holder->TriggerScriptEvent(26, (void *)reserveAmmo, false);
            AddAmo(properties->clipSize);
            DoneReloading();
        }
        break;
    case 2:
        shootType = (EWeaponShootType)1;
        if (!paused) {
            FireBullet();
            if (loadedAmmo <= 0) {
                StartReloading();
                DoneReloading();
            }
        }
        break;
    }
    if (forceProximityUpdate) {
        const float drop = 0.05f;
        CCollision collision;
        IVolume *world = (IVolume *)((ISceneNode *)g_scene.world)->GetWorldBoundingVolume((ISceneNode::EVolumeType)0);
        CVector3 bottom = transform.GetPos();
        bottom.z -= drop;
        CLine3 line(CVector3(bottom.x, bottom.y, bottom.z + 1.0f), bottom);
        CVolFiber fiber(line);
        CCDBObject::DisablePassThru(true);
        int result = world->TestCollision(fiber, collision, true);
        CCDBObject::DisablePassThru(false);
        if (result == 1) {
            transform.Translate(velocity);
            velocity.z = dt * -0.002727675484f + velocity.z;
        } else {
            velocity.z = 0.99f - collision.depth;
            transform.Translate(velocity);
            velocity.z = 0.0f;
            CVector3 right(collision.contacts->normal);
            CVector3 front = transform.GetFront();
            CVector3 up;
            SetCross(up, right, front);
            SetCross(front, up, right);
            Normalize(up);
            Normalize(front);
            transform.SetFront(front);
            transform.SetRight(right);
            transform.SetUp(up);
            forceProximityUpdate = 0;
        }
    }
    if (muzzleFlashStatus) {
        field_35c = MathFunRandomReal(0.0f, 12.56637061f);
        field_27 = 1;
        if (!field_30) {
            field_354 += dt;
            if (!(field_354 <= field_358)) {
                muzzleFlashStatus = 0;
                field_27 = 0;
                field_354 = 0.0f;
            }
        }
    }
}
