// Fragment of the fiber unit: the copy, endpoint and line constructors.
#include "Fiber.h"

CVolFiber::CVolFiber(const CVolFiber &other) : line(other.line) {}

CVolFiber::CVolFiber(const CVector3 &start, const CVector3 &end) { line.Set(start, end); }

CVolFiber::CVolFiber(const CLine3 &source) : line(source) {}

