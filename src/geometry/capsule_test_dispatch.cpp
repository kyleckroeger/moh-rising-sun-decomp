// Fragment of the capsule unit: double-dispatch tests. A fiber is tested as its line and
// an animated volume as a sphere.
#include "Capsule.h"
#include "AnimatedVolume.h"
#include "CDBObject.h"
#include "CSGVolume.h"
#include "Fiber.h"
#include "Sphere.h"
#include "WorldVolume.h"

EClsnState CVolCapsule::TestCollision(const CCDBObject &other, CCollision &collision, bool flag) const {
    collision.SwapOrder();
    return other.TestCollision(*this, collision, flag);
}

EClsnState CVolCapsule::TestCollision(const CVolFiber &other, CCollision &collision, bool flag) const {
    const CLine3 &line = other.GetLine();
    collision.lineTest = 1;
    return TestCollision(line, collision, flag);
}

EClsnState CVolCapsule::TestCollision(const CAnimatedVolume &other, CCollision &collision, bool flag) const {
    return TestCollision((const CVolSphere &)other, collision, flag);
}

EClsnState CVolCapsule::TestCollision(const CCSGVolume &other, CCollision &collision, bool flag) const {
    collision.SwapOrder();
    return other.TestCollision(*this, collision, flag);
}

EClsnState CVolCapsule::TestCollision(const CWorldVolume &other, CCollision &collision, bool flag) const {
    collision.SwapOrder();
    return other.TestCollision(*this, collision, flag);
}

