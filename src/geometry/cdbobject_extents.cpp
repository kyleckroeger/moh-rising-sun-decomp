// Fragment of the CDB object unit: the extents, and the fiber test that tests the fiber's
// line and records this object on the first contact.
#include "CDBObject.h"
#include "Fiber.h"

void CCDBObject::GetExtents(CVector3 &low, CVector3 &high) const {
    low = extentLow;
    high = extentHigh;
}

EClsnState CCDBObject::TestCollision(const CVolFiber &other, CCollision &collision, bool flag) const {
    collision.lineTest = 1;
    EClsnState state = TestCollision(other.GetLine(), collision, flag);
    if (state == 2 && collision.contacts)
        collision.contacts->subVolume = (int)this;
    return state;
}
