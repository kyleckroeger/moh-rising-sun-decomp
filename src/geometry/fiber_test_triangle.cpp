// Fragment of the fiber unit: fiber-triangle tests only set the line bit and return zero.
#include "Fiber.h"

EClsnState CVolFiber::TestCollision(const CTriangle &, CCollision &collision, bool) const {
    collision.lineTest = 1;
    return (EClsnState)0;
}
