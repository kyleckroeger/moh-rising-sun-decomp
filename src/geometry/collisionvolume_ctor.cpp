// Fragment of collisionvolume.cpp: the constructor clears the node fields and repeats the
// matrix class check.
#include "CollisionVolume.h"

CCollisionVolume::CCollisionVolume() {
    unknown_18[0] = 0;
    unknown_18[1] = 0;
    extentLowX = 0;
    extentHighX = 0;
    extentLowY = 0;
    extentHighY = 0;
    unknown_30[0] = 0;
    unknown_30[1] = 0;
    if (!CMatrix::s_ClassInit)
        CMatrix::InitClass();
    volume = 0;
}

