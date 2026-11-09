// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original Soldier_object.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "Bullet.h"
#include "AITargeting.h"
#include "GameSettings.h"
#include "ParticleClock.h"
#include "StringCRC.h"
#include "CameraShake.h"
#include "Volume.h"

extern "C" float sqrtf(float);
void *BSEventGetEventMemoryBlock(unsigned int);
void PlayImpactSound(unsigned short, unsigned short, const CVector3 &, bool);
long long MathFunRandomI64(long long, long long);
extern int BSObjectTriggerEvent(BSObject *, unsigned short, void *, BSObject *, bool);
extern int g_bInMultiplayerMode;
extern int g_bTeamDM;
extern ISceneNode *g_pDirectHitNode;

class CGrenadeResponseBehavior {
  public:
    static bool IsBeingHandled(CThrownBullet *);
};

struct SoldierHitInfo {
    BSObject *source;
    float damage;
    int direction;
    int bulletType;
    int location;
    int hitCount;
    unsigned char unknown_18[32 - 24];
};

static inline float LengthOf(const CVector3 &v) {
    float sum = v.x * v.x + v.y * v.y;
    sum += v.z * v.z;
    return sqrtf(sum);
}

static inline void Normalize(CVector3 &v) {
    float length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    if (length != 0.0f)
        v *= 1.0f / length;
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

static inline float Dot(const CVector3 &a, const CVector3 &b) {
    float sum = a.x * b.x + a.y * b.y;
    sum += a.z * b.z;
    return sum;
}

static inline void InitMatrix(CMatrix &m) {
    if (!CMatrix::s_ClassInit)
        CMatrix::InitClass();
    m.GetSlot(0);
}

void CSoldierObject::HandleBulletCollision(CBullet *bullet, const CCollision &collision) {
    if (bullet->AsProjectile() && bullet->AsProjectile()->GetScriptBulletType() == 28)
        return;
    CProjectileBullet *projectile = bullet->AsProjectile();
    CThrownBullet *thrown = bullet->AsThrown();
    if (projectile) {
        SoldierHitInfo *info = (SoldierHitInfo *)BSEventGetEventMemoryBlock(32);
        info->bulletType = bullet->GetScriptBulletType();
        info->damage = bullet->GetDamage();
        if (info->bulletType == 1 && hitTimer == 0.0f)
            hitTimer = 0.25f;
        if (!(hitTimer <= 0.0f))
            info->hitCount = ++hitCount;
        else
            info->hitCount = 1;
        ISceneNode *firer = bullet->GetFiredBy();
        if (firer)
            info->source = (BSObject *)firer->GetScriptObject();
        else
            info->source = 0;
        CVector3 direction, forward, right;
        ((ISceneNode *)bullet)->GetForward(direction);
        ((ISceneNode *)this)->GetForward(forward);
        ((ISceneNode *)this)->GetRightward(right);
        float front = Dot(forward, direction);
        float side = Dot(right, direction);
        if (!(side >= -0.7071f))
            info->direction = 3;
        else if (!(side <= 0.7071f))
            info->direction = 2;
        else if (!(front >= 0.0f))
            info->direction = 0;
        else
            info->direction = 1;
        const SClsnContact *contact = collision.contacts;
        int stopped = 0;
        info->location = contact->feature;
        switch (info->location) {
        case 3: case 4: case 5: case 6: case 7: case 8:
        case 13: case 14: case 15: case 16: case 17: case 18: case 19: case 20:
            if (!g_bInMultiplayerMode)
                info->damage *= 0.5f;
            break;
        case 0: case 1: {
            float scale = 1.0f;
            if (!g_bInMultiplayerMode) {
                if (MathFunRandomI64(0, 100) < helmetChance) {
                    stopped = EjectHelmet(bullet, collision);
                    if (stopped)
                        scale = helmetDamageScale;
                }
            }
            info->damage *= scale * 3.0f;
            break;
        }
        case 2: case 9: case 10: case 11: case 12:
            break;
        }
        if (g_cheats.oneShotKillsEnemies)
            info->damage = 1000.0f;
        if (g_cheats.headShotsOnly && (unsigned)info->location > 1)
            info->damage = 0.0f;
        if (g_bInMultiplayerMode && g_GameSettings.profile->field_32074 >= 0 && g_bTeamDM &&
            firer->AsMovingNode()->GetTeam() == ((IMovingSceneNode *)this)->GetTeam())
            info->damage = 0.0f;
        BSObjectTriggerEvent(scriptObject, 40, info, 0, false);
        if (firer && firer->AsPlayerObject() && !(firer->AsPlayerObject()->pathFlags & 0x8000) &&
            ((ISceneNode *)this)->GetAIObject() && ((ISceneNode *)this)->GetAIObject()->team == 2 &&
            info->hitCount <= 1) {
            g_GameSettings.Set_hits(1, false);
            switch (info->location) {
            case 3: case 4: case 5:
                g_GameSettings.Set_shotLocation(1, (EShotLocations)1, false);
                break;
            case 6: case 7: case 8:
                g_GameSettings.Set_shotLocation(1, (EShotLocations)2, false);
                break;
            case 13: case 14: case 15:
                g_GameSettings.Set_shotLocation(1, (EShotLocations)4, false);
                break;
            case 16: case 17: case 18:
                g_GameSettings.Set_shotLocation(1, (EShotLocations)5, false);
                break;
            case 19: case 20:
                g_GameSettings.Set_shotLocation(1, (EShotLocations)6, false);
                break;
            case 0: case 1:
                g_GameSettings.Set_shotLocation(1, (EShotLocations)0, false);
                break;
            case 2: case 9: case 10: case 11: case 12:
                g_GameSettings.Set_shotLocation(1, (EShotLocations)3, false);
                break;
            }
        }
        PlayImpactSound(stopped ? 1 : 41, 1, contact->position, false);
        IMovingSceneNode *blood = (IMovingSceneNode *)CPSManager::Create(GetStringCRC("enemyhit"), 1, (eParticleSystem)0);
        if (blood) {
            CMatrix matrix;
            InitMatrix(matrix);
            CVector3 up, across;
            CVector3 xAxis(1.0f, 0.0f, 0.0f);
            if (!(__builtin_fabsf(Dot(contact->normal, xAxis)) > 0.9999f)) {
                SetCross(across, contact->normal, xAxis);
                Normalize(across);
                SetCross(up, across, contact->normal);
                Normalize(up);
            } else {
                CVector3 zAxis(0.0f, 0.0f, 1.0f);
                SetCross(up, contact->normal, zAxis);
                Normalize(up);
                SetCross(across, contact->normal, up);
                Normalize(across);
            }
            CMatrix *m = &matrix;
            m->SetRight(up);
            m->SetFront(across);
            m->SetUp(contact->normal);
            m->SetPos(contact->position);
            blood->SetTMLocalToWorld(*m);
        }
    } else if (thrown) {
        if (!CGrenadeResponseBehavior::IsBeingHandled(thrown)) {
            if (g_bInMultiplayerMode)
                g_pDirectHitNode = (ISceneNode *)this;
            thrown->Explode();
        }
    }
}
