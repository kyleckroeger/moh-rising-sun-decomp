// AI-assisted reconstruction from GR8E69; see docs/Bullets.md.
#include "Bullet.h"
#include "IDestructible.h"
#include "Light.h"
class CGrenadeResponseBehavior {
  public:
    static void GrenadeDestroyed(CThrownBullet *);
};
void CThrownBullet::MarkForDestruction(int delay) {
    if (type == 1 || type == 2) CGrenadeResponseBehavior::GrenadeDestroyed(this);
    ISubject::MarkForDestruction(delay);
    if (light) {
        ((ISubject *)light.operator->())->MarkForDestruction(delay);
        light.Assign(0);
    }
    if (trail) {
        trail->DeActivate();
        trail = 0;
    }
    noCatch = 1;
    g_scene.Remove(*this);
    collisionId = (EClsnId)-1;
}
