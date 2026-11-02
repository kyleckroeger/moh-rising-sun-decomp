// Fragment of the fiber unit: the endpoint getters, the line accessor and the tests where
// the other volume tests against this fiber's line.
#include "Fiber.h"
#include "AnimatedVolume.h"
#include "CDBObject.h"
#include "CSGVolume.h"
#include "Box.h"
#include "Capsule.h"
#include "Sphere.h"
#include "WorldVolume.h"

CVector3 CVolFiber::GetStart() const { return line.GetStart(); }

CVector3 CVolFiber::GetEnd() const { return line.GetEnd(); }

const CLine3 &CVolFiber::GetLine() const { return line; }

EClsnState CVolFiber::TestCollision(const IVolume &other, CCollision &collision, bool flag) const {
    collision.SwapOrder();
    return other.TestCollision(*this, collision, flag);
}

EClsnState CVolFiber::TestCollision(const CVolBox &other, CCollision &collision, bool flag) const {
    collision.SwapOrder();
    collision.lineTest = 1;
    return other.TestCollision(*this, collision, flag);
}

EClsnState CVolFiber::TestCollision(const CVolSphere &other, CCollision &collision, bool flag) const {
    collision.SwapOrder();
    collision.lineTest = 1;
    return other.TestCollision(*this, collision, flag);
}

EClsnState CVolFiber::TestCollision(const CVolCapsule &other, CCollision &collision, bool flag) const {
    collision.SwapOrder();
    collision.lineTest = 1;
    return other.TestCollision(*this, collision, flag);
}

EClsnState CVolFiber::TestCollision(const CCDBObject &other, CCollision &collision, bool flag) const {
    collision.SwapOrder();
    collision.lineTest = 1;
    return other.TestCollision(*this, collision, flag);
}

EClsnState CVolFiber::TestCollision(const CAnimatedVolume &other, CCollision &collision, bool flag) const {
    collision.SwapOrder();
    collision.lineTest = 1;
    return other.TestCollision(*this, collision, flag);
}

EClsnState CVolFiber::TestCollision(const CCSGVolume &other, CCollision &collision, bool flag) const {
    collision.SwapOrder();
    collision.lineTest = 1;
    return other.TestCollision(*this, collision, flag);
}

EClsnState CVolFiber::TestCollision(const CWorldVolume &other, CCollision &collision, bool flag) const {
    collision.SwapOrder();
    collision.lineTest = 1;
    return other.TestCollision(*this, collision, flag);
}

