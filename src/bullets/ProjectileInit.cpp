// AI-assisted reconstruction from GR8E69; see docs/Bullets.md.
#include "Bullet.h"
#include "CameraShake.h"
#include "Fiber.h"
#include "Volume.h"
extern "C" float sqrtf(float);
float MathFunRandomReal(float, float);
extern int g_bInMultiplayerMode;
extern int g_bCoopMultiplayer;
extern int g_NumPlayers;
#define OWNED (*(WeakPtr<ISceneNode, 8> *)ownedStorage)
#define FIBER(p) ((CVolFiber *)(p))
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
void CProjectileBullet::Init(EProjectileBulletTypes type_, ProjectileBulletProperties_struct *properties_,
                             const CVector3 &start, const CVector3 &direction, float speed_, EClsnId id, float,
                             ISceneNode *owner, bool keep) {
    CBullet::Init(keep);
    if (keep && OWNED) g_scene.IncludeCollision(*OWNED.operator->(), *this);
    type = type_;
    properties = properties_;
    field_b0 = properties_->lifeFrames;
    speed = speed_ * properties_->weight;
    float seconds = properties_->lifeSeconds;
    collisionTime = -1.0f;
    life = seconds;
    field_100 = MathFunRandomReal(0.0f, 0.5f);
    position = start;
    prevPosition = start;
    collisionId = id;
    OWNED.Assign(owner);
    excluded.Assign(0);
    flags.hitPending = 0;
    flags.hit = 0;
    flags.shutDown = 0;
    if (type_ == 30) flags.drawForward = 1;
    else flags.drawForward = 0;
    trail = 0;
    localToWorld.BuildTrans(start);
    CVector3 front(direction);
    CVector3 right, up;
    const CVector3 &n = front;
    float length = sqrtf(front.LengthSquared());
    if (length != 0.0f) front *= 1.0f / length;
    if (!(__builtin_fabs(n.z) >= __builtin_fabs(n.x))) {
        up.Set(0.0f, 0.0f, 1.0f);
        SetCross(right, front, up);
        SetCross(up, right, front);
    } else {
        if (!(front.y >= 0.0f)) right.Set(-1.0f, 0.0f, 0.0f);
        else right.Set(1.0f, 0.0f, 0.0f);
        SetCross(up, right, front);
        SetCross(right, front, up);
    }
    localToWorld.SetFront(front);
    localToWorld.SetRight(right);
    localToWorld.SetUp(up);
    if (!keep) g_scene.Add(*this, 0);
    FIBER(localVolume)->Set(CVector3(0.0f, 0.0f, 0.0f), CVector3(0.0f, 0.0f, 0.0f));
    CPlayerObject *player = g_scene.GetPlayer(0);
    FIBER(worldVolume)->CVolFiber::TransformedCopy(*FIBER(localVolume), localToWorld);
    if (g_bInMultiplayerMode || g_bCoopMultiplayer) {
        for (int i = 0; i < g_NumPlayers; i++) {
            player = g_scene.GetPlayer(i);
            ISceneNode *body = (ISceneNode *)((char *)player + 3168);
            if (keep) g_scene.IncludeCollision(*this, *body);
            IVolume *volume = (IVolume *)body->GetWorldBoundingVolume((ISceneNode::EVolumeType)0);
            CCollision collision;
            if (volume->TestCollision(*FIBER(worldVolume), collision, false) != 1) g_scene.ExcludeCollision(*this, *body, -1);
        }
    } else {
        ISceneNode *body = (ISceneNode *)((char *)player + 3168);
        if (keep) g_scene.IncludeCollision(*this, *body);
        IVolume *volume = (IVolume *)body->GetWorldBoundingVolume((ISceneNode::EVolumeType)0);
        CCollision collision;
        if (volume->TestCollision(*FIBER(worldVolume), collision, false) != 1) g_scene.ExcludeCollision(*this, *body, -1);
    }
}
