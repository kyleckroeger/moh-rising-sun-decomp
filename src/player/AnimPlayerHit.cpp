// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
#include "CameraShake.h"
#include "Bullet.h"
#include "ParticleClock.h"
#include "ParticleRecipe.h"
#include "StringCRC.h"
#include "Volume.h"
extern "C" float sqrtf(float);
static inline void SetCross(CVector3 &out, const CVector3 &a, const CVector3 &b) {
    CVector3 r;
    r.x = a.y * b.z - a.z * b.y;
    r.y = a.z * b.x - a.x * b.z;
    r.z = a.x * b.y - a.y * b.x;
    out.x = r.x;
    out.y = r.y;
    out.z = r.z;
}
static inline void Normalize(CVector3 &v) {
    float length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    if (length != 0.0f) {
        float scale = 1.0f / length;
        v.x *= scale;
        v.y *= scale;
        v.z *= scale;
    }
}
static inline float fabsf(float x) { return __builtin_fabsf(x); }
static inline float DotOf(const CVector3 &a, const CVector3 &b) {
    float sum = a.x * b.x + a.y * b.y;
    sum += a.z * b.z;
    return sum;
}
void CAnimatedPlayer::HandleBulletCollision(CBullet *bullet, const CCollision &collision) {
    if (bullet->AsProjectile()) {
        const SClsnContact *contact = collision.contacts;
        IParticleSystem *blood = CPSManager::Create(GetStringCRC("EnemyHit"), true, (eParticleSystem)0);
        if (blood) {
            CMatrix matrix;
            CMatrix *m = &matrix;
            if (!CMatrix::s_ClassInit) CMatrix::InitClass();
            m->GetSlot(0);
            CVector3 right, front;
            CVector3 axis(1.0f, 0.0f, 0.0f);
            if (!(fabsf(DotOf(contact->normal, axis)) > 0.9999f)) {
                SetCross(front, contact->normal, axis);
                Normalize(front);
                SetCross(right, front, contact->normal);
                Normalize(right);
            } else {
                CVector3 up(0.0f, 0.0f, 1.0f);
                SetCross(right, contact->normal, up);
                Normalize(right);
                SetCross(front, contact->normal, right);
                Normalize(front);
            }
            matrix.SetRight(right);
            matrix.SetFront(front);
            matrix.SetUp(contact->normal);
            matrix.SetPos(contact->position);
            blood->SetTMLocalToWorld(matrix);
        }
    }
    ((ISceneNode *)player)->HandleBulletCollision(bullet, collision);
}
