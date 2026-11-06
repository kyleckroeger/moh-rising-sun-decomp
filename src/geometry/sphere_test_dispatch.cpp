// Fragment of sphere.cpp: a sphere's double-dispatch tests. An animated volume is tested
// as a sphere and a fiber as its line.
#include "Sphere.h"
#include "AnimatedVolume.h"
#include "CDBObject.h"
#include "CSGVolume.h"
#include "Capsule.h"
#include "Fiber.h"
#include "WorldVolume.h"

EClsnState CVolSphere::TestCollision(const CAnimatedVolume &other, CCollision &collision, bool flag) const {
    return TestCollision((const CVolSphere &)other, collision, flag);
}

EClsnState CVolSphere::TestCollision(const CCSGVolume &other, CCollision &collision, bool flag) const {
    collision.SwapOrder();
    return other.TestCollision(*this, collision, flag);
}

EClsnState CVolSphere::TestCollision(const CWorldVolume &other, CCollision &collision, bool flag) const {
    collision.SwapOrder();
    return other.TestCollision(*this, collision, flag);
}

EClsnState CVolSphere::TestCollision(const CVolCapsule &other, CCollision &collision, bool flag) const {
    collision.SwapOrder();
    return other.TestCollision(*this, collision, flag);
}

EClsnState CVolSphere::TestCollision(const CVolFiber &other, CCollision &collision, bool flag) const {
    collision.lineTest = 1;
    return TestCollision(other.GetLine(), collision, flag);
}

EClsnState CVolSphere::TestCollision(const CCDBObject &other, CCollision &collision, bool flag) const {
    collision.SwapOrder();
    return other.TestCollision(*this, collision, flag);
}

