// Fragment of animated_volume.cpp: tests that run as a sphere or swap the order, then
// Create and TransformedCopy. Members at +0x20, +0x24 and +0x30 are unnamed.
#include "AnimatedVolume.h"
#include "CDBObject.h"
#include "CSGVolume.h"
#include "Box.h"
#include "Capsule.h"
#include "Fiber.h"
#include "Sphere.h"
#include "WorldVolume.h"

EClsnState CAnimatedVolume::TestCollision(const IVolume &other, CCollision &collision, bool flag) const {
    collision.SwapOrder();
    return other.TestCollision(*this, collision, flag);
}

EClsnState CAnimatedVolume::TestCollision(const CVolBox &other, CCollision &collision, bool flag) const {
    return CVolSphere::TestCollision(other, collision, flag);
}

EClsnState CAnimatedVolume::TestCollision(const CVolSphere &other, CCollision &collision, bool flag) const {
    return CVolSphere::TestCollision(other, collision, flag);
}

EClsnState CAnimatedVolume::TestCollision(const CVolCapsule &other, CCollision &collision, bool flag) const {
    return CVolSphere::TestCollision(other, collision, flag);
}

EClsnState CAnimatedVolume::TestCollision(const CCDBObject &other, CCollision &collision, bool flag) const {
    collision.SwapOrder();
    return other.TestCollision(*this, collision, flag);
}

EClsnState CAnimatedVolume::TestCollision(const CVector3 &other, CCollision &collision, bool flag) const {
    return CVolSphere::TestCollision(other, collision, flag);
}

EClsnState CAnimatedVolume::TestCollision(const CPlane &other, CCollision &collision, bool flag) const {
    return CVolSphere::TestCollision(other, collision, flag);
}

EClsnState CAnimatedVolume::TestCollision(const CTriangle &other, CCollision &collision, bool flag) const {
    return CVolSphere::TestCollision(other, collision, flag);
}

EClsnState CAnimatedVolume::TestCollision(const CVolFiber &other, CCollision &collision, bool flag) const {
    collision.lineTest = 1;
    return TestCollision(other.GetLine(), collision, flag);
}

EClsnState CAnimatedVolume::TestCollision(const CWorldVolume &other, CCollision &collision, bool flag) const {
    collision.SwapOrder();
    return other.TestCollision(*this, collision, flag);
}

EClsnState CAnimatedVolume::TestCollision(const CCSGVolume &other, CCollision &collision, bool flag) const {
    collision.SwapOrder();
    return other.TestCollision(*this, collision, flag);
}

EClsnState CAnimatedVolume::TestCollision(const CAnimatedVolume &other, CCollision &collision, bool flag) const {
    return CVolSphere::TestCollision((const CVolSphere &)other, collision, flag);
}

void *AllocateAnimatedVolume();
inline void *operator new(unsigned int, void *p) { return p; }

IVolume *CAnimatedVolume::Create() const { return new (AllocateAnimatedVolume()) CAnimatedVolume; }

void CAnimatedVolume::TransformedCopy(const IVolume &other, const CMatrix &matrix) {
    CVolSphere::TransformedCopy(other, matrix);
    const CAnimatedVolume &volume = (const CAnimatedVolume &)other;
    unknown_24 = volume.unknown_24;
    unknown_20 = 0;
    unknown_30 = volume.unknown_30;
}
