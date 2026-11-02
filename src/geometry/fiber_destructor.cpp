// Fragment of the fiber unit: the destructor is CVolFiber's key function, so its table is
// emitted here. The compiler also emits the inline default constructor, which the original
// link does not keep; its literal remains at 0x802a8490.
#include "Fiber.h"

CVolFiber::~CVolFiber() {}
