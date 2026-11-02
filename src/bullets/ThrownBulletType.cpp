// AI-assisted reconstruction from GR8E69; see docs/Bullets.md.
#include "Bullet.h"
void DebugMsg(const char *, ...);
int CThrownBullet::GetScriptBulletType() const {
    switch (type) {
    case 0:
    case 6:
    case 8:
    case 9:
    case 10:
    case 11:
        return 21;
    case 2:
    case 4:
    case 7:
        return 24;
    default:
        DebugMsg("WARNING: Unknown thrown bullet type in GetScriptBulletType.  Type %d\n", type);
    case 1:
    case 5:
        return 3;
    }
}
