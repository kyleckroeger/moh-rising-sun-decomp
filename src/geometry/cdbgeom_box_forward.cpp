// Fragment of cdbgeom.cpp: CDB box tests that forward to the plain box tests. The table
// stays original context because the destructor is emitted elsewhere.
#include "CDBGeom.h"
#include "CDBObject.h"
#include "WorldVolume.h"

EClsnState CCDBVolBox::TestCollision(const IVolume &other, CCollision &collision, bool flag) const {
    return CVolBox::TestCollision(other, collision, flag);
}

EClsnState CCDBVolBox::TestCollision(const CVolBox &other, CCollision &collision, bool flag) const {
    return CVolBox::TestCollision(other, collision, flag);
}

EClsnState CCDBVolBox::TestCollision(const CCDBObject &other, CCollision &collision, bool flag) const {
    return CVolBox::TestCollision(other, collision, flag);
}

EClsnState CCDBVolBox::TestCollision(const CVector3 &other, CCollision &collision, bool flag) const {
    return CVolBox::TestCollision(other, collision, flag);
}

EClsnState CCDBVolBox::TestCollision(const CWorldVolume &, CCollision &, bool) const { return (EClsnState)0; }
