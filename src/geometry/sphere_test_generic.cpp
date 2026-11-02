// Fragment of sphere.cpp: a sphere swaps the collision order and lets a generic volume or
// box run the test.
#include "Sphere.h"
#include "Box.h"

EClsnState CVolSphere::TestCollision(const IVolume &other, CCollision &collision, bool flag) const {
    collision.SwapOrder();
    return other.TestCollision(*this, collision, flag);
}

EClsnState CVolSphere::TestCollision(const CVolBox &other, CCollision &collision, bool flag) const {
    collision.SwapOrder();
    return other.TestCollision(*this, collision, flag);
}

