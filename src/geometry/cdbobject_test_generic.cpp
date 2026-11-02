// Fragment of the CDB object unit: the destructor (the key function, so the table is
// emitted here) and the generic volume test.
#include "CDBObject.h"

CCDBObject::~CCDBObject() {}

EClsnState CCDBObject::TestCollision(const IVolume &other, CCollision &collision, bool flag) const {
    collision.SwapOrder();
    return other.TestCollision(*this, collision, flag);
}

