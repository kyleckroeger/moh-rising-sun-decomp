// Fragment of the capsule unit: Create allocates a default-constructed capsule.
#include "Capsule.h"

IVolume *CVolCapsule::Create() const { return new CVolCapsule; }
