// Fragment of fiber.cpp: visibility tests a capsule of radius 0.3 around the line.
#include "Fiber.h"
#include "Frustum.h"
#include "Capsule.h"

int CVolFiber::TestVisibility(CDrawContext &context) const {
    CVolCapsule capsule;
    CVolCapsule *c = &capsule;
    CLine3 &target = c->line;
    target = line;
    c->radius = 0.3f;
    return c->CVolCapsule::TestVisibility(context);
}
