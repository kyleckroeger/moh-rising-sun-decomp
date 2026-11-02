// Fragment of cdbgeom.cpp: a CDB box tests an animated volume as a sphere, a capsule as a
// plain box and does not test CSG volumes.
#include "CDBGeom.h"
#include "AnimatedVolume.h"
#include "CDBObject.h"
#include "CSGVolume.h"
#include "Capsule.h"
#include "Fiber.h"
#include "WorldVolume.h"

EClsnState CCDBVolBox::TestCollision(const CAnimatedVolume &other, CCollision &collision, bool flag) const {
    return TestCollision((const CVolSphere &)other, collision, flag);
}

EClsnState CCDBVolBox::TestCollision(const CCSGVolume &, CCollision &, bool) const { return (EClsnState)0; }

EClsnState CCDBVolBox::TestCollision(const CVolCapsule &other, CCollision &collision, bool flag) const {
    return CVolBox::TestCollision(other, collision, flag);
}
