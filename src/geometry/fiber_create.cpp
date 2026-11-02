// Fragment of the fiber unit: Create allocates a default-constructed fiber.
#include "Fiber.h"

IVolume *CVolFiber::Create() const { return new CVolFiber; }
