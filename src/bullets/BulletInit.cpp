// AI-assisted reconstruction from GR8E69; see docs/Bullets.md.
#include "Bullet.h"
void CBullet::Init(bool keep) {
    localToWorld.GetSlot(0);
    field_b0 = 0.0f;
    collisionId = static_cast<EClsnId>(-1);
    field_b4 = 3.402823466e+38f;
    if (!keep) {
        field_1c = keep;
        field_18 = keep;
    }
}
