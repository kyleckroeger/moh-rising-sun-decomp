// Fragment of the capsule unit: the destructor is CVolCapsule's key function, so its table
// is emitted here. The compiler also emits the inline default constructor, which the
// original link does not keep; its literal remains at 0x802a858c.
#include "Capsule.h"

CVolCapsule::~CVolCapsule() {}
