// Fragment of cdbgeom.cpp: a CDB box tests a fiber as its line.
#include "CDBGeom.h"
#include "Fiber.h"

EClsnState CCDBVolBox::TestCollision(const CVolFiber &other, CCollision &collision, bool flag) const {
    collision.lineTest = 1;
    return TestCollision(other.GetLine(), collision, flag);
}
