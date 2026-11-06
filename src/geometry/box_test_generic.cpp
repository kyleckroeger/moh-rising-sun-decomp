// Fragment of box.cpp: a box swaps the collision order and lets a generic volume run the
// test.
#include "Box.h"

EClsnState CVolBox::TestCollision(const IVolume &other, CCollision &collision, bool flag) const {
    collision.SwapOrder();
    return other.TestCollision(*this, collision, flag);
}

