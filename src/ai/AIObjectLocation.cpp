// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Fragments of the original AIObject.cpp; member names are descriptive.
#include "AITargeting.h"

void CAIObject::GetLocation(float *x, float *y, float *z) {
    const CVector3 *location = &position;
    *x = location->x;
    *y = location->y;
    *z = location->z;
}

void CAIObject::GetLocation(CVector3 *out) { GetLocation(&out->x, &out->y, &out->z); }

float CAIObject::GetDistanceXYZReal(CAIObject *other) { return position.Distance(other->position); }

float CAIObject::GetDistanceXYReal(CAIObject *other) { return position.DistanceXY(other->position); }
