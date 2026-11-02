// Fragment of world_volume.cpp: the sub-volume dispatch, extents and the unimplemented
// tests that return zero.
#include "WorldVolume.h"
#include "AnimatedVolume.h"
#include "CDBObject.h"
#include "CSGVolume.h"
#include "Box.h"
#include "Capsule.h"
#include "Fiber.h"
#include "Sphere.h"

EClsnState CWorldVolume::TestCollision(const IVolume &other, CCollision &collision, bool flag) const {
    collision.SwapOrder();
    return other.TestCollision(*this, collision, flag);
}

EClsnState CWorldVolume::TestCollision(const CVolBox &other, CCollision &collision, bool flag) const {
    return TestCollisionWithSubVolumes(other, collision, flag);
}

EClsnState CWorldVolume::TestCollision(const CVolSphere &other, CCollision &collision, bool flag) const {
    return TestCollisionWithSubVolumes(other, collision, flag);
}

EClsnState CWorldVolume::TestCollision(const CVolCapsule &other, CCollision &collision, bool flag) const {
    return TestCollisionWithSubVolumes(other, collision, flag);
}

EClsnState CWorldVolume::TestCollision(const CLine3 &other, CCollision &collision, bool flag) const {
    return TestCollisionWithSubVolumes(other, collision, flag);
}

EClsnState CWorldVolume::TestCollision(const CVolFiber &other, CCollision &collision, bool flag) const {
    return TestCollisionWithSubVolumes(other, collision, flag);
}

EClsnState CWorldVolume::TestCollision(const CAnimatedVolume &other, CCollision &collision, bool flag) const {
    return TestCollisionWithSubVolumes(other, collision, flag);
}

EClsnState CWorldVolume::TestCollision(const CCSGVolume &other, CCollision &collision, bool flag) const {
    collision.SwapOrder();
    return other.TestCollision(*this, collision, flag);
}

void CWorldVolume::GetExtents(CVector3 &low, CVector3 &high) const {
    low = extentLow;
    high = extentHigh;
}

EClsnState CWorldVolume::TestCollision(const CCDBObject &, CCollision &, bool) const { return (EClsnState)0; }
EClsnState CWorldVolume::TestCollision(const CWorldVolume &, CCollision &, bool) const { return (EClsnState)0; }
EClsnState CWorldVolume::TestCollision(const CVector3 &, CCollision &, bool) const { return (EClsnState)0; }
EClsnState CWorldVolume::TestCollision(const CPlane &, CCollision &, bool) const { return (EClsnState)0; }
EClsnState CWorldVolume::TestCollision(const CTriangle &, CCollision &, bool) const { return (EClsnState)0; }
