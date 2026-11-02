// AI-assisted reconstruction from GR8E69; see docs/Bullets.md.
#include "Bullet.h"
void CProjectileBullet::GetVelocity(CVector3 &value) {
    CVector3 direction(localToWorld.GetFront());
    const CVector3 &front = direction;
    float scale = speed;
    value.x = front.x * scale;
    value.y = front.y * scale;
    value.z = front.z * scale;
}
