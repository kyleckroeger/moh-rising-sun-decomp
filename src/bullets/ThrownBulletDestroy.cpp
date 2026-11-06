// AI-assisted reconstruction from GR8E69; see docs/Bullets.md.
#include "Bullet.h"
class CBulletFactory {
  public:
    void DestroyBullet(CBullet *);
};
extern CBulletFactory *g_pBulletFactory;
void CThrownBullet::Destroy() { g_pBulletFactory->DestroyBullet(this); }
