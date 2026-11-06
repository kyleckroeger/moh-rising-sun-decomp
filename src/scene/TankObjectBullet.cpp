// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original Tank_object.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "Bullet.h"
#include "ParticleClock.h"
#include "StringCRC.h"
#include "Volume.h"

long long MathFunGetRandomPercent();
int MathFunTestPercent(long long, long long);

void CTankObject::HandleBulletCollision(CBullet *bullet, const CCollision &collision) {
    if (bullet->AsProjectile()) {
        CMatrix matrix;
        CMatrix *saved = &matrix;
        if (!CMatrix::s_ClassInit)
            CMatrix::InitClass();
        const SClsnContact *contact = collision.contacts;
        saved->GetSlot(0);
        saved->SetPos(contact->position);
        if (MathFunTestPercent(MathFunGetRandomPercent(), 30)) {
            IMovingSceneNode *hit = (IMovingSceneNode *)CPSManager::Create(GetStringCRC("bullethit"), 1, eParticleSystemDefault);
            if (hit)
                hit->SetTMLocalToWorld(*saved);
        }
        for (int i = 0; i < 2; i++) {
            IMovingSceneNode *spark;
            if (MathFunTestPercent(MathFunGetRandomPercent(), 65))
                spark = (IMovingSceneNode *)CPSManager::Create(GetStringCRC("Bullet_Sparks_One"), 1, eParticleSystemDefault);
            else
                spark = (IMovingSceneNode *)CPSManager::Create(GetStringCRC("Bullet_Sparks_Two"), 1, eParticleSystemDefault);
            if (spark)
                spark->SetTMLocalToWorld(matrix);
        }
    }
    ((CStaticObject *)this)->HandleBulletCollision(bullet, collision);
}
