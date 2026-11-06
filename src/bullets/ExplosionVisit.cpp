// AI-assisted reconstruction from GR8E69; see docs/Bullets.md.
#include "Bullet.h"
#include "CameraShake.h"
#include "GameSettings.h"
#include "Shell.h"
#include "ScriptTimers.h"
#include "WorldObject.h"
#include "CLine3.h"
extern "C" float sqrtf(float);
extern "C" int rand();
class CAIObject;
void MakeNoise(void *, CAIObject *, const CVector3 &, int, float, bool, int);
void *BSEventGetEventMemoryBlock(unsigned int);
extern int g_bInMultiplayerMode;
extern int g_bCoopMultiplayer;
extern ISceneNode *g_pDirectHitNode;
struct BS_STRUCT_HitByBulletInfo_struct {
    BSObject *source;
    float damage;
    int front;
    int bulletType;
    int location;
    int unknown_14[3];
};

static inline float Dot(const CVector3 &a, const CVector3 &b) {
    float sum = a.x * b.x + a.y * b.y;
    sum += a.z * b.z;
    return sum;
}
static inline float Length(const CVector3 &v) { return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z); }
CExplosion::~CExplosion() {}
bool CExplosion::Visit(ISceneNode &node) {
    if (node.GetScriptObject()) {
        if (CheckForObstructions(node) || node.AsAnimatedPlayerObject() || node.AsPlayerWeaponObject()) return false;
        float range = 0.8f * sphere.GetRadius();
        BS_STRUCT_HitByBulletInfo_struct *info = (BS_STRUCT_HitByBulletInfo_struct *)BSEventGetEventMemoryBlock(32);
        float falloff;
        CVector3 delta;
        
        
        CVector3 forward;
        node.GetForward(forward);
        node.GetPosition(delta);
        delta -= sphere.GetCenter();
        if (!(Length(delta) <= 1.0f)) {
            float length = Length(delta);
            falloff = 1.0f - (length - 1.0f) / sphere.GetRadius();
        }
        else falloff = 1.0f;
        if (!(falloff >= 0.5f)) falloff = 0.5f;
        if (g_bInMultiplayerMode && g_pDirectHitNode == &node) {
            falloff = 1.0f;
            g_pDirectHitNode = 0;
        }
        info->damage = damage * falloff;
        info->bulletType = bulletType;
        if (source) info->source = (BSObject *)source->GetScriptObject();
        else info->source = 0;
        float facing = Dot(forward, delta);
        if (!(facing > 0.0f)) info->front = 0;
        else info->front = 1;
        if (node.AsPlayerObject()) {
            if (node.AsPlayerObject()->IsInvincible()) info->damage = 0.0f;
            else if (g_cheats.oneHitKills) info->damage = 1000.0f;
            if (g_bInMultiplayerMode || g_bCoopMultiplayer) {
                if (!(node.AsPlayerObject()->field_d54 <= 0.0f)) info->damage = 0.0f;
            }
            if (g_bInMultiplayerMode && info->source) {
                if (info->front == 1) info->location = 10;
                else info->location = 9;
                node.AsPlayerObject()->SetMPShotLocation(info);
            }
        }
        if (node.AsMovingNode() && !(Length(delta) >= range)) {
            CPlayerObject *unused = node.AsPlayerObject();
            if (!BSObjectTriggerEvent((BSObject *)node.GetScriptObject(), 178, info, 0, false)) {
                BS_STRUCT_HitByBulletInfo_struct *copy = (BS_STRUCT_HitByBulletInfo_struct *)BSEventGetEventMemoryBlock(32);
                if (copy) {
                    *copy = *info;
                    if (!g_bInMultiplayerMode) copy->bulletType = 5;
                    if (info->front == 1) copy->location = 10;
                    else copy->location = 9;
                    BSObjectTriggerEvent((BSObject *)node.GetScriptObject(), 40, copy, 0, false);
                }
            }
        } else if (node.AsHierObject()) {
            info->location = 0;
            BSObjectTriggerEvent((BSObject *)node.GetScriptObject(), 178, info, 0, false);
        } else if (node.AsCollisionVolume()) {
            BSObjectTriggerEvent((BSObject *)node.GetScriptObject(), 40, info, 0, false);
        } else {
            if (!g_bInMultiplayerMode) info->bulletType = 5;
            info->location = rand() % 21;
            BSObjectTriggerEvent((BSObject *)node.GetScriptObject(), 40, info, 0, false);
        }
        CPlayerObject *player = node.AsPlayerObject();
        if (player) {
            IMovingSceneNode *moving = (IMovingSceneNode *)player;
            CVector3 playerForward, playerRight;
            moving->GetForward(playerForward);
            moving->GetRightward(playerRight);
            float ahead = Dot(playerForward, delta);
            float side = Dot(playerRight, delta);
            if (!(side >= -0.7071f)) player->hud.AddHitDirection(2);
            else if (!(side <= 0.7071f)) player->hud.AddHitDirection(4);
            else if (!(ahead >= 0.0f)) player->hud.AddHitDirection(1);
            else player->hud.AddHitDirection(3);
            g_inputMgr.SetFeedback(moving->GetPlayerIndex(), 2, 0, falloff, 1.0f);
            player->SetCameraShake(falloff, 0.0f, 1.0f, 10.0f);
        }
    } else {
        CBullet *bullet = node.AsBullet();
        if (bullet) {
            CThrownBullet *thrown = bullet->AsThrown();
            if (thrown) {
                if (CheckForObstructions(node)) return false;
                thrown->Explode();
            }
        }
    }
    return false;
}

