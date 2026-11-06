// AI-assisted reconstruction from GR8E69; see docs/Bullets.md.
#include "Bullet.h"
class CBulletFactory {
  public:
    ProjectileBulletProperties_struct *projectileProperties;
    ThrownBulletProperties_struct *thrownProperties;
    float GetBulletWeight(bool, int);
};
float CBulletFactory::GetBulletWeight(bool projectile, int index) {
    if (projectile) return projectileProperties[index].weight;
    return thrownProperties[index].weight;
}
