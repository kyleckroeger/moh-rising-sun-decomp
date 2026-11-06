// Fragment of the CDB object unit: tests that are not implemented return zero.
#include "CDBObject.h"
#include "WorldVolume.h"

EClsnState CCDBObject::TestCollision(const CCDBObject &, CCollision &, bool) const { return (EClsnState)0; }
EClsnState CCDBObject::TestCollision(const CWorldVolume &, CCollision &, bool) const { return (EClsnState)0; }
EClsnState CCDBObject::TestCollision(const CVector3 &, CCollision &, bool) const { return (EClsnState)0; }
EClsnState CCDBObject::TestCollision(const CPlane &, CCollision &, bool) const { return (EClsnState)0; }
EClsnState CCDBObject::TestCollision(const CTriangle &, CCollision &, bool) const { return (EClsnState)0; }
