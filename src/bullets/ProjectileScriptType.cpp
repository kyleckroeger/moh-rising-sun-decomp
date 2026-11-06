// AI-assisted reconstruction from GR8E69; see docs/Bullets.md.
#include "Bullet.h"
void DebugMsg(const char *, ...);
int CProjectileBullet::GetScriptBulletType() const {
    switch (type) {
    case 5:
    case 7:
        return 0;
    case 10:
        return 12;
    case 11:
    case 12:
        return 13;
    case 13:
        return 11;
    case 15:
    case 16:
        return 5;
    case 18:
    case 20:
        return 6;
    case 21:
    case 22:
        return 1;
    case 23:
    case 24:
        return 20;
    case 28:
    case 29:
    case 33:
        return 2;
    case 19:
    case 27:
        return 17;
    case 34:
        return 27;
    case 14:
        return 28;
    case 6:
        return 29;
    case 26:
        return 30;
    case 25:
        return 31;
    default:
        DebugMsg("WARNING: Unknown proj bullet type in GetScriptBulletType.  Type %d\n", type);
    case 8:
    case 9:
        return 4;
    }
}
