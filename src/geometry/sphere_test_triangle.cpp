// Fragment of sphere.cpp: sphere-triangle tests are not implemented and return zero.
#include "Sphere.h"

EClsnState CVolSphere::TestCollision(const CTriangle &, CCollision &, bool) const { return (EClsnState)0; }
