// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
#include "CameraShake.h"
#include "Bullet.h"
#include "GameSettings.h"
#include "MathFun.h"
#include "ScriptTimers.h"
#include "Shell.h"
void *BSEventGetEventMemoryBlock(unsigned int);
void PlayImpactSound(unsigned short, unsigned short, const CVector3 &, bool);
extern int g_bInMultiplayerMode;
extern int g_bTeamDM;
struct BS_STRUCT_HitByBulletInfo_struct {
    BSObject *source;
    float damage;
    int direction;
    int bulletType;
    int location;
    unsigned char unknown_14[32 - 20];
};
static inline float Dot(const CVector3 &a, const CVector3 &b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
void CPlayerObject::HandleBulletCollision(CBullet *bullet, const CCollision &collision) {
    if (flags.headingOverride) return;
    bool grenade = false;
    CProjectileBullet *projectile = bullet->AsProjectile();
    CThrownBullet *thrown = bullet->AsThrown();
    if (thrown && (thrown->type == 4 || thrown->type == 7)) grenade = true;
    if (projectile || grenade) {
        BS_STRUCT_HitByBulletInfo_struct *info = (BS_STRUCT_HitByBulletInfo_struct *)BSEventGetEventMemoryBlock(32);
        info->bulletType = bullet->GetScriptBulletType();
        info->damage = bullet->GetDamage();
        ISceneNode *firer = bullet->GetFiredBy();
        if (firer) info->source = (BSObject *)firer->GetScriptObject();
        else info->source = (BSObject *)firer;
        CVector3 direction, forward, right;
        const CVector3 &d = direction;
        bullet->GetForward(direction);
        ((ISceneNode *)this)->GetForward(forward);
        ((ISceneNode *)this)->GetRightward(right);
        float front = Dot(forward, d);
        float side = Dot(right, d);
        if (!(side >= -0.7071f)) {
            hud.AddHitDirection(2);
            info->direction = 3;
        } else if (!(side <= 0.7071f)) {
            hud.AddHitDirection(4);
            info->direction = 2;
        } else if (!(front >= 0.0f)) {
            hud.AddHitDirection(1);
            info->direction = 0;
        } else {
            hud.AddHitDirection(3);
            info->direction = 1;
        }
        if (IsInvincible()) info->damage = 0.0f;
        else if (g_cheats.oneHitKills) info->damage = 1000.0f;
        if (g_bInMultiplayerMode) {
            int team = firer->AsMovingNode()->GetTeam();
            if (g_bTeamDM && g_GameSettings.profile->field_32074 >= 0 && team == ((IMovingSceneNode *)this)->GetTeam())
                info->damage = 0.0f;
            if (!(field_d54 <= 0.0f)) info->damage = 0.0f;
            int location = collision.contacts->feature;
            info->location = location;
            if (location >= 0) {
                if (location <= 1) info->damage *= 3.0f;
            }
            SetMPShotLocation(info);
        }
        BSObjectTriggerEvent(script, 40, info, 0, false);
        float strength = MathFunClamp(info->damage * 0.05f, 0.0f, 1.0f);
        if (g_bInMultiplayerMode) g_inputMgr.SetFeedback((unsigned char)playerIndex, 12, 0, strength, 0.1f);
        else g_inputMgr.SetFeedback((unsigned char)playerIndex, 12, 0, strength, 0.1f);
        CVector3 position;
        ((ISceneNode *)bullet)->GetPosition(position);
        unsigned short kind = 1;
        if (thrown) {
            kind = 2;
            if (thrown->type == 4) kind = 6;
        }
        PlayImpactSound(41, kind, position, true);
        if (thrown && thrown->type == 4) thrown->MarkForDestruction(0);
    }
    if (g_bInMultiplayerMode && thrown) thrown->Explode();
}
