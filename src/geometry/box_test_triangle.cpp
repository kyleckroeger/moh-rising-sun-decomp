// Fragment of box.cpp: box-triangle tests are not implemented and return zero.
#include "Box.h"

EClsnState CVolBox::TestCollision(const CTriangle &, CCollision &, bool) const { return (EClsnState)0; }
