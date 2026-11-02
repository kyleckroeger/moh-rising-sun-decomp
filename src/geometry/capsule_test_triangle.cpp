// Fragment of the capsule unit: capsule-triangle tests are not implemented and return
// zero.
#include "Capsule.h"

EClsnState CVolCapsule::TestCollision(const CTriangle &, CCollision &, bool) const { return (EClsnState)0; }
