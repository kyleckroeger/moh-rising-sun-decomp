// Fragment of csg_volume.cpp: the order-swapping test of a generic volume.
#include "CSGVolume.h"

EClsnState CCSGVolume::TestCollision(const IVolume &other, CCollision &collision, bool flag) const {
    collision.SwapOrder();
    return other.TestCollision(*this, collision, flag);
}

