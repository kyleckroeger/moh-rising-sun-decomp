// AI-assisted reconstruction from GR8E69; see docs/Bullets.md.
#include "Thrown.h"
#include "IDestructible.h"
#include "Bullet.h"
#include "CQuaternion.h"
extern "C" float sqrtf(float);
class BSGO_Basic;
void ForceUpdateProximityTriggerStatus(BSGO_Basic *, bool);
void PlayImpactSound(unsigned short, unsigned short, const CVector3 &, bool);
static inline void SetScaled(CVector3 &r, const CVector3 &v, float s) { r.x = v.x * s; r.y = v.y * s; r.z = v.z * s; }
static inline void SetSum(CVector3 &r, const CVector3 &a, const CVector3 &b) { r.x = a.x + b.x; r.y = a.y + b.y; r.z = a.z + b.z; }
static inline void SetDiff(CVector3 &r, const CVector3 &a, const CVector3 &b) { r.x = a.x - b.x; r.y = a.y - b.y; r.z = a.z - b.z; }
static inline float Dot(const CVector3 &a, const CVector3 &b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
static inline float SumOfSquares(const CVector3 &v) {
    float sum = v.x * v.x + v.y * v.y;
    sum += v.z * v.z;
    return sum;
}
// Only the fields Draw writes; the complete class is unknown.
class CDrawContext {
  public:
    unsigned char unknown_0[192];
    CMatrix *matrix;
    unsigned char unknown_c4[252 - 196];
    int field_fc;
};
// The mesh record behind CStaticObject::model, with its table pointer at +24.
class ThrownModel {
  public:
    unsigned char unknown_0[24];
    virtual void unknown_08();
    virtual void unknown_10();
    virtual void unknown_18();
    virtual void Draw(CDrawContext &, int, int, int, int);
};
static inline void SetCross(CVector3 &out, const CVector3 &a, const CVector3 &b) {
    CVector3 r;
    r.x = a.y * b.z - a.z * b.y;
    r.y = a.z * b.x - a.x * b.z;
    r.z = a.x * b.y - a.y * b.x;
    out.x = r.x;
    out.y = r.y;
    out.z = r.z;
}
static inline void PostMultiply(CMatrix &m, const CMatrix &o) {
    CMatrix::s_TempMat = m;
    m.Multiply(CMatrix::s_TempMat, o);
}
void CThrownObject::Draw(CDrawContext &context) {
    CMatrix rotation;
    if (!CMatrix::s_ClassInit) CMatrix::InitClass();
    float minSpeed = definition->data->minRollSpeed->value;
    if (!(sqrtf(velocity.LengthSquared()) <= minSpeed)) {
        CVector3 up(0.0f, 0.0f, 1.0f);
        CVector3 axis;
        SetCross(axis, up, velocity);
        IVolume *sphere = worldVolume;
        float length = sqrtf(axis.LengthSquared());
        if (length != 0.0f) axis *= 1.0f / length;
        float radius;
        if (sphere) radius = sphere->GetRadius();
        else radius = 1.0f;
        rotation.BuildRot(axis, sqrtf(stepVelocity.LengthSquared()) / radius);
        CQuaternion turn;
        turn.SetFromMatrix(rotation);
        rollOrientation.Multiply(turn);
    }
    rollOrientation.GetMatrix(rotation);
    PostMultiply(rotation, transform);
    context.matrix = &rotation;
    context.field_fc = drawArgument;
    ((ThrownModel *)model)->Draw(context, motionStart, 3, 4, 9);
    context.field_fc = 0;
}
void CThrownObject::BeginUpdate(float dt) {
    lightVolumes.Update(dt);
    animPosition = transform.GetPos();
    lastVelocity = velocity;
    ForceUpdateProximityTriggerStatus((BSGO_Basic *)scriptNode, true);
    if (pendingSound > 0) {
        if (impactSound == 1) PlayImpactSound(*(unsigned short *)((char *)&pendingSound + 2), 4, animPosition, false);
        pendingSound = -1;
    }
    if (!(lifetime <= 0.0f) && state) {
        lifetime -= dt;
        if (!(lifetime >= 0.0f)) lifetime = 0.0f;
    }
}
void CThrownObject::AttemptUpdate(float dt) {
    if (bits.deleted) return;
    field_320.Zero();
    if (bits.inContact && state) velocity.z = lastVelocity.z + dt * -0.002727675484f;
    IVolume *volume = worldVolume;
    if (field_20c != -1) {
        float speed = sqrtf(velocity.LengthSquared());
        float limit = volume->GetRadius() * 2.0f;
        if (!(limit >= speed)) velocity *= limit / speed;
    }
    SetScaled(stepVelocity, velocity, dt);
    CVector3 next;
    SetSum(next, animPosition, stepVelocity);
    transform.SetPos(next);
    ((ISceneNode *)this)->UpdateCollisionVolumes();
}
void CThrownObject::CommitUpdate() {
    if (bits.deleted) return;
    if (!(lifetime <= 0.0f)) {
        if (worldVolumeType3 != (IVolume *)field_13c && worldVolumeType3 && worldVolume)
            worldVolumeType3->TransformedCopy(*worldVolume, transform);
        animPosition = transform.GetPos();
        lastVelocity = velocity;
        if (bits.touched) bits.inContact = 0;
        else bits.inContact = 1;
        bits.touched = 0;
        bits.touched2 = 0;
    } else {
        ((ISubject *)this)->MarkForDestruction(0);
    }
}
void CThrownObject::HandleBulletCollision(CBullet *bullet, const CCollision &collision) {
    if (bullet->AsProjectile() && state == -1) {
        CVector3 impulse;
        bullet->GetVelocity(impulse);
        impulse *= definition->data->impulseScale->value;
        AddVelocity(impulse);
        SClsnContact *contact = collision.contacts;
        if (impactSound == 1) PlayImpactSound(1, 1, contact->position, false);
    }
}
