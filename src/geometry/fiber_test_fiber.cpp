// Fragment of the fiber unit: a fiber is tested as its line.
#include "Fiber.h"

EClsnState CVolFiber::TestCollision(const CVolFiber &other, CCollision &collision, bool flag) const {
    collision.lineTest = 1;
    return TestCollision(other.GetLine(), collision, flag);
}
