// Fragment of the capsule unit: the order-swapping tests of a generic volume and a box.
#include "Capsule.h"
#include "Box.h"

EClsnState CVolCapsule::TestCollision(const IVolume &other, CCollision &collision, bool flag) const {
    collision.SwapOrder();
    return other.TestCollision(*this, collision, flag);
}

EClsnState CVolCapsule::TestCollision(const CVolBox &other, CCollision &collision, bool flag) const {
    collision.SwapOrder();
    return other.TestCollision(*this, collision, flag);
}

