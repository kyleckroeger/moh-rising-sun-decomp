// AI-assisted reconstruction from GR8E69; see docs/Bullets.md.
#include "Bullet.h"
#include "Volume.h"
class CBulletFactory {
  public:
    void DestroyBullet(CBullet *);
};
extern CBulletFactory *g_pBulletFactory;
void CProjectileBullet::MarkForDestruction(int delay) {
    ISubject::MarkForDestruction(delay);
    flags.hitPending = 1;
    g_scene.Remove(*this);
    if (trail) {
        trail->DeActivate();
        trail = 0;
    }
}
void CProjectileBullet::Destroy() { g_pBulletFactory->DestroyBullet(this); }
void CProjectileBullet::Shutdown() {
    flags.shutDown = 1;
    CBullet::Shutdown();
}
void CProjectileBullet::OnCollision(const CCollision &collision) {
    if (!flags.hit) {
        ISceneNode *other = (ISceneNode *)(collision.first != (void *)this ? collision.first : collision.second);
        flags.hitPending = 1;
        other->HandleBulletCollision(this, collision);
        if (flags.hitPending) {
            flags.hit = 1;
            flags.hitPending = 0;
        } else {
            collisionTime = collision.unknown_08;
        }
    }
}
