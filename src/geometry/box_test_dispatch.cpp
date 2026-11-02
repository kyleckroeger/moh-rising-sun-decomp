// Fragment of box.cpp: a box's double-dispatch tests. A point is tested as a zero-radius
// sphere and a fiber as its line.
#include "Box.h"
#include "AnimatedVolume.h"
#include "CDBObject.h"
#include "CSGVolume.h"
#include "Fiber.h"
#include "Sphere.h"
#include "WorldVolume.h"

EClsnState CVolBox::TestCollision(const CCDBObject &other, CCollision &collision, bool flag) const {
    collision.SwapOrder();
    return other.TestCollision(*this, collision, flag);
}

EClsnState CVolBox::TestCollision(const CWorldVolume &other, CCollision &collision, bool flag) const {
    collision.SwapOrder();
    return other.TestCollision(*this, collision, flag);
}

EClsnState CVolBox::TestCollision(const CCSGVolume &other, CCollision &collision, bool flag) const {
    collision.SwapOrder();
    return other.TestCollision(*this, collision, flag);
}

EClsnState CVolBox::TestCollision(const CVector3 &point, CCollision &collision, bool flag) const {
    CVolSphere sphere(0.005f, point);
    return TestCollision(sphere, collision, flag);
}

EClsnState CVolBox::TestCollision(const CVolFiber &other, CCollision &collision, bool flag) const {
    collision.lineTest = 1;
    return TestCollision(other.GetLine(), collision, flag);
}

EClsnState CVolBox::TestCollision(const CAnimatedVolume &other, CCollision &collision, bool flag) const {
    collision.SwapOrder();
    return other.TestCollision(*this, collision, flag);
}

