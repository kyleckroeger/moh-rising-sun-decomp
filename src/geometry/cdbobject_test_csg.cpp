// Fragment of the CDB object unit: the CSG volume test swaps the collision order.
#include "CDBObject.h"
#include "CSGVolume.h"

EClsnState CCDBObject::TestCollision(const CCSGVolume &other, CCollision &collision, bool flag) const {
    collision.SwapOrder();
    return other.TestCollision(*this, collision, flag);
}

