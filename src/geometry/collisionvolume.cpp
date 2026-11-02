// Most of collisionvolume.cpp: a scene node wrapping a volume and a matrix. Bullet hits
// trigger script event 40 with the side struck. The destructor, the key function, and the
// table stay original context because the destructor inlines ~ISceneNode, which the shared
// header declares out of line.
#include "CollisionVolume.h"
#include "Bullet.h"
#include "ScriptRuntime.h"
#include "ScriptTimers.h"

void *BSEventGetEventMemoryBlock(unsigned int);

void CCollisionVolume::Init(IVolume *newVolume, CMatrix *matrix, EClsnId id) {
    volume = newVolume;
    localToWorld = *matrix;
    collisionId = id;
}

void CCollisionVolume::GetTMLocalToWorld(CMatrix &out) const { out = localToWorld; }

void CCollisionVolume::GetPosition(CVector3 &out) const { out = localToWorld.GetPos(); }

void CCollisionVolume::GetRightward(CVector3 &out) const { out = localToWorld.GetRight(); }

void CCollisionVolume::GetForward(CVector3 &out) const { out = localToWorld.GetFront(); }

void CCollisionVolume::GetUpward(CVector3 &out) const { out = localToWorld.GetUp(); }

int CCollisionVolume::GetWorldBoundingVolume(ISceneNode::EVolumeType) const { return (int)volume; }

int CCollisionVolume::GetLocalBoundingVolume(ISceneNode::EVolumeType) const { return (int)volume; }

void CCollisionVolume::BeginUpdate(float) {
    if (volume) {
        CVector3 low;
        CVector3 high;
        volume->GetExtents(low, high);
        extentLowX->value = low.x;
        extentLowY->value = low.y;
        extentHighX->value = high.x;
        extentHighY->value = high.y;
    } else {
        CVector3 position;
        GetPosition(position);
        extentLowX->value = position.x;
        extentLowY->value = position.y;
        extentHighX->value = position.x;
        extentHighY->value = position.y;
    }
}

// The event block's layout is read from this function; the name is descriptive.
struct BulletHitEvent {
    int shooter;
    float damage;
    int side;
    int type;
    int unknown_10[4];
};

void CCollisionVolume::HandleBulletCollision(CBullet *bullet, const CCollision &) {
    if (!bullet->AsProjectile())
        return;
    BulletHitEvent *event = (BulletHitEvent *)BSEventGetEventMemoryBlock(sizeof(BulletHitEvent));
    event->type = bullet->GetScriptBulletType();
    event->damage = bullet->GetDamage();
    ISceneNode *shooter = bullet->GetFiredBy();
    if (shooter)
        event->shooter = shooter->GetScriptObject();
    else
        event->shooter = 0;
    CVector3 hitDirection;
    CVector3 &direction = hitDirection; // Plain reference: reproduces the code shape.
    CVector3 forward;
    CVector3 right;
    bullet->GetForward(direction);
    GetForward(forward);
    GetRightward(right);
    float front = forward.x * direction.x + forward.y * direction.y + forward.z * direction.z;
    float side = right.x * direction.x + right.y * direction.y + right.z * direction.z;
    if (!(side >= -0.7071f))
        event->side = 3;
    else if (!(side <= 0.7071f))
        event->side = 2;
    else if (!(front >= 0.0f))
        event->side = 0;
    else
        event->side = 1;
    BSObjectTriggerEvent(script, 40, event, 0, false);
}

void CCollisionVolume::MarkForDestruction(int flag) {
    ISubject::MarkForDestruction(flag);
    g_scene.Remove(*this);
}

void CCollisionVolume::Destroy() {
    if (script && script->gameObject)
        script->gameObject->Destroy();
}

EClsnId CCollisionVolume::GetCollisionId() const { return collisionId; }

CCollisionVolume *CCollisionVolume::AsCollisionVolume() { return this; }

const CCollisionVolume *CCollisionVolume::AsCollisionVolume() const { return this; }

void CCollisionVolume::SetScript(BSObject *newScript) { script = newScript; }

int CCollisionVolume::GetScriptObject() const { return (int)script; }

void CCollisionVolume::SetCollisionId(EClsnId id) { collisionId = id; }
